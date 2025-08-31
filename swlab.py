# chord_notes.py
# Build a chord (set of note names) from a musical key (root) and chord type.

# 12-tone chromatic scale using sharps
NOTES = ["C", "C#", "D", "D#", "E", "F",
         "F#", "G", "G#", "A", "A#", "B"]

# Accept flat names by mapping them to enharmonic sharps
ENHARMONIC = {
    "DB": "C#", "EB": "D#", "GB": "F#",
    "AB": "G#", "BB": "A#"
}

# Dictionary of chord types → intervals (in semitones from the root)
CHORD_STEPS = {
    "major":              [4, 7],
    "minor":              [3, 7],
    "augmented fifth":    [4, 8],
    "minor fifth":        [4, 6],   # triad with a flat 5 (a.k.a. C–E–Gb for C)
    "major sixth":        [4, 7, 9],
    "minor sixth":        [3, 7, 9],
    "dominant seventh":   [4, 7, 10],
    "minor seventh":      [3, 7, 10],
    "major seventh":      [4, 7, 11],
    "diminished seventh": [3, 6, 10],
    # (optional) common aliases:
    "maj": [4, 7], "min": [3, 7], "dim7": [3, 6, 10], "maj7": [4, 7, 11], "7": [4, 7, 10]
}

def normalize_note(name: str) -> str:
    """Normalize user input to one of NOTES (convert flats to sharps, uppercase, trim)."""
    x = name.strip().upper().replace(" ", "")
    return ENHARMONIC.get(x, x)

def chord_notes(root: str, chord_type: str):
    """Return a list like ['C','E','G'] for a given root and chord type."""
    root_norm = normalize_note(root)
    if root_norm not in NOTES:
        raise ValueError(f"Unknown root note: {root}")
    steps = CHORD_STEPS.get(chord_type.strip().lower())
    if steps is None:
        # try again without case and collapse spaces (to match keys like 'major seventh')
        key = " ".join(chord_type.lower().split())
        steps = CHORD_STEPS.get(key)
    if steps is None:
        raise ValueError(f"Unknown chord type: {chord_type}")

    i0 = NOTES.index(root_norm)
    # Start with the root (0 semitones), then add each step modulo 12
    idxs = [i0] + [ (i0 + s) % 12 for s in steps ]
    return [NOTES[i] for i in idxs]

if __name__ == "__main__":
    root = input("Enter the key (e.g., C, D#, Eb, F): ")
    ctype = input("Enter chord type (e.g., major, minor, major seventh, dominant seventh): ")
    try:
        notes = chord_notes(root, ctype)
        print("Notes:", ", ".join(notes))
    except ValueError as e:
        print("Error:", e)

