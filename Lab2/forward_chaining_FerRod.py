from typing import List, Tuple

Antecedent = List[str]
Rule = Tuple[Antecedent, str]

def forward_chain(DB: List[str], KB: List[Rule], verbose: bool = True) -> List[str]:
    """runs a basic forward chaining algorithm"""
    DB = list(DB)
    count = 1
    changes = True

    while changes:
        changes = False
        if verbose:
            print("\nstarting round " + str(count))
        for p in KB:
            antecedent, consequent = p
            if verbose:
                print("checking rule:", antecedent, "=>", consequent)

            # check if every condition in the rule is already known
            satisfied = True
            for q in antecedent:
                if q not in DB:
                    satisfied = False
                    break

            # if rule conditions are met and we don't already know the result
            if satisfied and consequent not in DB:
                DB.append(consequent)
                changes = True
                if verbose:
                    print("new fact learned:", consequent)
                    print("DB so far:", DB)
            elif satisfied and consequent in DB:
                if verbose:
                    print("we already knew", consequent)
            else:
                if verbose:
                    print("can't conclude anything here yet")
        count += 1

    if verbose:
        print("\ndone! final DB:")
        print(DB)

    return DB

def main():
    # stuff we already know
    DB = ["looks", "swims", "quacks"]

    # rules we want to check against
    KB: List[Rule] = [
        (["looks", "swims", "quacks"], "duck"),
        (["barks"], "dog"),
        (["hoots", "flies"], "owl"),
    ]

    # run the forward chaining process
    forward_chain(DB, KB, verbose=True)

if __name__ == "__main__":
    main()
