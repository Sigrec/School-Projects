/**
 * One edge of the flights (only used by FlightMap class)
 */
class FlightPath {
	protected String startingCity, destination;
	protected int val;
	protected FlightPath next;

	protected FlightPath(String startingCity, String destination, int val) {
		this.startingCity = startingCity;
		this.destination = destination;
		this.val = val;
		next = null;
	}
}
