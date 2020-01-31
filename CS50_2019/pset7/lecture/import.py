import csv

# Open TSV file for reading
with open("title.basics.tsv", "r") as titles:
    reader = csv.DictReader(titles, delimiter="\t")

    with open ("shows2.csv", "w") as shows:
        writer = csv.writer(shows)
        writer.writerow(["tconst", "primaryTitle", "startYear", "genres"])

        # Iterate over CSV file
        for row in reader:
            if row["startYear"] != "\\N":
                if int(row["startYear"]) >= 1970:
                    if row["titleType"] == "tvSeries" and row["isAdult"] == "0":
                        writer.writerow([row["tconst"], row["primaryTitle"], row["startYear"], row["genres"]])

