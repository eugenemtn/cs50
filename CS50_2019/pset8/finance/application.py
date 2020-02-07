import os
import time

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, datetime_filter

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filters
app.jinja_env.filters["usd"] = usd
app.jinja_env.filters["datetime"] = datetime_filter


# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    cols = [ "Symbol", "Name", "Shares", "Price", "TOTAL"]
    available_cash = db.execute(
        "SELECT cash FROM users WHERE users.id = :user_id",
        user_id=session["user_id"]
    )[0]["cash"]
    stocks_owned = db.execute(
        "SELECT * FROM user_stocks WHERE user_id=:user_id",
        user_id=session["user_id"]
    )
    total = available_cash

    for corp in stocks_owned:
        current_corp = lookup(corp["symbol"])
        if current_corp:
            corp["current_price"] = float(current_corp["price"])
            corp["total_price"] = corp["current_price"] * int(corp["stocks"])
            total += float(corp["total_price"])

    return render_template(
        "index.html",
        stocks_owned=stocks_owned,
        cash=available_cash,
        cols=cols,
        total=total
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        if not symbol:
            return apology("Symbol is missing")
        if not shares:
            return apology("Shares amount is missing")
        if shares <= 0:
            return apology("Shares amount could be positive only")
        quote = lookup(symbol)

        if not quote:
            return apology("Invalid symbol")

        # Get available cash
        available_cash = db.execute(
            "SELECT cash FROM users WHERE users.id = :user_id",
            user_id=session["user_id"]
        )[0]["cash"]

        # If it is enough cash to get these stocks
        if quote["price"] * shares > available_cash:
            return apology("You have not enough cash")

        # Find stocks for this user in owned stocks
        stocks_in_portfolio = db.execute(
            "SELECT * FROM user_stocks WHERE user_id = :user_id AND symbol = :symbol",
            user_id=session["user_id"], symbol=quote["symbol"]
        )

        #
        if len(stocks_in_portfolio) != 1:
            db.execute(
                "INSERT  INTO user_stocks (user_id, stocks, symbol, company_name) VALUES (?, ?, ?, ?)",
                session["user_id"], shares, quote["symbol"], quote["name"]
            )
        elif len(stocks_in_portfolio) == 1:
            db.execute(
                "UPDATE user_stocks SET stocks = :stocks WHERE user_id = :user_id and symbol = :symbol",
                stocks=int(stocks_in_portfolio[0]["stocks"]) + shares,
                user_id=session["user_id"],
                symbol=quote["symbol"]
            )
        db.execute(
            "UPDATE users SET cash = :cash WHERE id = :user_id",
            cash=available_cash - quote["price"] * shares,
            user_id=session["user_id"]
        )
        db.execute(
            "INSERT INTO transactions (user_id, symbol, stock_price, stocks, timestamp) VALUES (?, ?, ?, ?, ?)",
            session["user_id"], quote["symbol"], quote["price"], shares, int(time.time())
        )
        flash("Bought!")
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute(
        "SELECT * FROM transactions WHERE user_id = :user_id ORDER BY timestamp DESC",
        user_id=session["user_id"]
    )

    return render_template(
        "history.html",
        transactions=transactions if len(transactions) > 0 else None
    )


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST
    if request.method == "POST":
        symbol = request.form.get("symbol")

        # Ensure quote is submitted
        if not symbol:
            return apology("No symbol was provided")

        quote = lookup(symbol)
        quote["price"] = float(quote["price"])
        if quote:
         return render_template("quote.html", quote=quote)
        else:
            return apology("Invalid symbol")
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("You must provide a username")

        # Ensure both password and its confirmation were submitted
        if not password or not confirmation:
            return apology("You must provide both a password and its confirmation")

        # Ensure that password and its confirmation match
        if password != confirmation:
            return apology("Passwords do not match")

        # Ensure that username is available for registration
        check_username = db.execute("SELECT 'username' FROM users WHERE username=:username", username=username)
        if len(check_username) == 0:
            user_id = db.execute(f"INSERT INTO users (username, hash) VALUES (?, ?)",
                username, generate_password_hash(password))
            session["user_id"] = user_id
            return redirect("/")
        else:
            return apology("User already exists")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    def get_stocks():

        # Find stocks for this user in owned stocks
        user_stocks = db.execute(
            "SELECT * FROM user_stocks WHERE user_id = :user_id",
            user_id=session["user_id"]
        )
        return user_stocks

    if request.method == "POST":
        company = request.form.get("company")
        shares = int(request.form.get("shares"))
        if not company:
            return apology("Company field is required")
        if not shares:
            return apology("Shares field is required")
        if shares <= 0:
            return apology("Shares should be positive integer")
        quote = lookup(company)
        if not quote:
            return apology("Invalid symbol")
        user_stocks = get_stocks()
        stocks_to_sell = next(stock for stock in user_stocks if stock["symbol"] == company)
        if stocks_to_sell["stocks"] < shares:
            return apology("Not enough stocks owned")

        # Get available cash
        available_cash = db.execute(
            "SELECT cash FROM users WHERE users.id = :user_id",
            user_id=session["user_id"]
        )[0]["cash"]
        new_stocks_count = int(stocks_to_sell["stocks"]) - shares

        if new_stocks_count > 0:
            db.execute(
                "UPDATE user_stocks SET stocks = :stocks WHERE user_id = :user_id and symbol = :symbol",
                stocks=new_stocks_count,
                user_id=session["user_id"],
                symbol=company
            )
        elif new_stocks_count == 0:
            db.execute(
                "DELETE FROM user_stocks WHERE user_id = :user_id AND symbol = :symbol",
                user_id=session["user_id"],
                symbol=company
            )
        db.execute(
            "UPDATE users SET cash = :cash WHERE id = :user_id",
            cash=int(available_cash) + int(quote["price"]) * shares,
            user_id=session["user_id"]
        )
        db.execute(
            "INSERT INTO transactions (user_id, symbol, stock_price, stocks, timestamp) VALUES (?, ?, ?, ?, ?)",
            session["user_id"], company, quote["price"], f"-{shares}", int(time.time())
        )
        flash("Sold!")
        return redirect("/")
    else:
        return render_template("sell.html", stocks=get_stocks())
    return apology("TODO")

@app.route("/add-cash", methods=["POST"])
@login_required
def add_cash():
    """Add some cash to the account"""

    cash = int(request.form.get("cash"))

    if not cash:
        return apology("Cash is required")
    if cash <= 0:
        return apology("Cash should be positive")

    # Get available cash
    available_cash = db.execute(
        "SELECT cash FROM users WHERE users.id = :user_id",
        user_id=session["user_id"]
    )[0]["cash"]

    db.execute(
        "UPDATE users SET cash = :cash WHERE id = :user_id",
        cash=float(available_cash) + float(request.form.get("cash")),
        user_id=session["user_id"]
    )
    flash("Cash added!")
    return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
