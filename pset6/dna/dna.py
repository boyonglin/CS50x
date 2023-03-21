import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # Read database file into a variable
    STRs = []
    profiles = []

    with open(sys.argv[1], "r") as database:
        reader = csv.DictReader(database)
        # Store list of Short Tandem Repeats (STRs)
        STRs = reader.fieldnames[1:]
        for row in reader:
            profiles.append(row)

    # Read DNA sequence file into a variable
    f = open(sys.argv[2], "r")
    sequence = f.read()

    # Find longest match of each STR in DNA sequence
    STR_match = dict.fromkeys(STRs, [])

    for STR in STRs:
        STR_match[STR] = longest_match(sequence, STR)

    # Check database for matching profiles
    for profile in profiles:
        match_count = 0

        for STR in STRs:
            if int(profile[STR]) == STR_match[STR]:
                match_count += 1

        if match_count == len(STRs):
            print(profile["name"])
            return 0

    print("No match")
    return 1


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
