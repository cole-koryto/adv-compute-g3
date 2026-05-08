import csv
import sys


def main():
    if len(sys.argv) != 2:
        print("Usage: plot.py <phase5_bench.csv>")
        return 2

    path = sys.argv[1]
    sizes = []
    totals = []

    with open(path, newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            sizes.append(int(row["orders"]))
            totals.append(float(row["total_s"]))

    try:
        import matplotlib.pyplot as plt
    except Exception as e:
        print("matplotlib is required to plot:", e)
        return 1

    plt.plot(sizes, totals, marker="o", linestyle="-", color="b")
    plt.xlabel("Number of Orders")
    plt.ylabel("Execution Time (seconds)")
    plt.title("HFT Order Book Performance (add+modify+delete)")
    plt.grid(True)
    plt.show()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

