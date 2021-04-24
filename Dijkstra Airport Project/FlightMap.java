/**
 * Builds a flight map made out cities
 */
import java.util.*;

class FlightMap {
	private final HashMap<String, City> flights;

	/**
	 * Builds a flight plan from a set of paths
	 * @param paths : The FlightList of paths for a given city
	 */
	public FlightMap(FlightList<FlightPath> paths) {
		flights = new HashMap<>(paths.getSize());

		//Find adjacent cities
		for (int x = 0; x < paths.getSize(); x++) {
			if (!flights.containsKey(paths.get(x).startingCity)){ //Checks to see if the starting city is already in the list of cities
				flights.put(paths.get(x).startingCity, new City(paths.get(x).startingCity));
			}
			if (!flights.containsKey(paths.get(x).destination)){ //Checks to see if the destination city is already in the list of cities
				flights.put(paths.get(x).destination, new City(paths.get(x).destination));
			}
		}

		// Sets neighboring paths
		for (int x = 0; x < paths.getSize(); x++) {
			flights.get(paths.get(x).startingCity).adjacentCities.put(flights.get(paths.get(x).destination), paths.get(x).val);
			flights.get(paths.get(x).destination).adjacentCities.put(flights.get(paths.get(x).startingCity), paths.get(x).val); //Comment this out if the paths are undirected
		}
	}

	/**
	 * Runs dijkstra shortest path algorithm using a specified starting city
	 * @param sourceCity : The starting city to determine shortest path for
	 */
	protected void findShortestPath(String sourceCity) {
		if (!flights.containsKey(sourceCity)) { //Checks to see if the flight is in the flight plan and if not print error
			System.err.printf("Flight doesn't contain start vertex \"%s\"\n", sourceCity);
			return;
		}
		//Get all the flights for the starter city
		City source = flights.get(sourceCity);
		source.val = 0;

		//Get the paths and add it to Priority Queue
		PriorityQueue<City> cityPaths = new PriorityQueue<>();
		//FlightQueue<City> cityPaths = new FlightQueue<>();
		for (City curr : flights.values()) {
			curr.prev = curr == source ? source : null;
			curr.val = curr == source ? 0 : Integer.MAX_VALUE;
			cityPaths.offer(curr);
		}

		City startingCity, destinationCity;
		// vertex with shortest val (first iteration will return source)
		while (!cityPaths.isEmpty()) {
			// vertex with shortest val (first iteration will return source)
			startingCity = cityPaths.poll();
			if (startingCity.val == Integer.MAX_VALUE){
				break;
			}
			//System.out.println(startingCity.toString());

			//Find time/cost of each neighboring edge
			for (HashMap.Entry<City, Integer> city : startingCity.adjacentCities.entrySet()) {
				destinationCity = city.getKey(); //The neighbor
				int alternateDist = startingCity.val + city.getValue();
				//System.out.println("Test: " + alternateDist);
				if (alternateDist < destinationCity.val) { //Checks to see if a shorter path is found
					//cityPaths.remove(destinationCity);
					destinationCity.val = alternateDist;
					destinationCity.prev = startingCity;
					cityPaths.offer(destinationCity);
				}
			}
		}
	}

	/**
	 * Gets the total value, either time or cost, for a flight path
	 * @param destination
	 * @return int : the total value
	 */
	protected int getTotal(String destination){
		return flights.get(destination).val;
	}

	/**
	 * Prints a path from the source to the specified vertex
	 * @param destination : The destination the flight path will end on
	 */
	protected void printPath(String destination) {
		if (!flights.containsKey(destination)) {
			System.err.printf("Graph doesn't contain end vertex \"%s\"\n", destination);
			return;
		}
		flights.get(destination).printPath();
	}
}