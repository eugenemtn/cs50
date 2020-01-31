import csv

# Open TSV file for reading
with open("title.basics.tsv", "r") as titles:
    reader = csv.DictReader(titles, delimiter="\t")

    with open ("genres2.csv", "w") as shows:
        writer = csv.writer(shows)
        writer.writerow(["show_id", "genre"])

        # Iterate over CSV file
        for row in reader:
            if row["startYear"] == "\\N":
                continue
            if int(row["startYear"]) < 1970:
                continue

            if row["titleType"] == "tvSeries" and row["isAdult"] == "0":
                if row["genres"] != "\\N":
                    for genre in row["genres"].split(","):
                        writer.writerow([row["tconst"], genre])

