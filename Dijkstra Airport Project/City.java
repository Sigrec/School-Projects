import java.util.HashMap;
import java.util.Map;

/**
 * One vertex/neighboring city of city and maps to neighboring cities
 */
class City implements Comparable<City> {
	private final String cityName;
	protected int val = Integer.MAX_VALUE;
	protected City prev = null;
	protected final Map<City, Integer> adjacentCities = new HashMap<>();

	protected City(String cityName) {
		this.cityName = cityName;
	}

	/**
	 * Prints the shortest path
	 */
	protected void printPath() {
		if (this == this.prev) {
			System.out.print(this.cityName);
		} else if (this.prev == null) {
			System.out.print("(unreached)" + this.cityName);
		} else {
			this.prev.printPath();
			System.out.print(" -> " + this.cityName);
		}
	}

	/**
	 * Compares two cities based on there time or cost
	 * @param other : The other city to compare to
	 * @return int : the comparator value
	 */
	public int compareTo(City other) {
		if (val == other.val)
			return cityName.compareTo(other.cityName);
		return Integer.compare(val, other.val);
	}

	@Override
	public String toString() {
		return "(" + cityName + ", " + val + ")";
	}
}