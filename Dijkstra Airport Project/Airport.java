/**
 * Creates an "airport" flight map by taking data from two files and outputs the shortest or smallest path based on
 * what the file/user wants to see
 */
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.StringTokenizer;

public class Airport {
	public static void main(String[] args) {
		//Gets the data flight path data
		FlightList<FlightPath> costMap = new FlightList<>();
		FlightList<FlightPath> timeMap = new FlightList<>();
		getFlightPathData(costMap, timeMap);

		//Creates flight map with the flight path data and find the shortest path based on input from user file
		FlightMap costFlightMap = new FlightMap(costMap);
		FlightMap timeFlightMap = new FlightMap(timeMap);
		getRequestedFlights(costFlightMap, timeFlightMap);
	}

	/**
	 * Gets the .txt file of all the flights and there destinations w/ val & time then stores destinations in a custom linked list mapped to the starting flight
	 * @param costMap : The map of all the flights based on cost
	 * @param timeMap : The map of all the flights based on time
	 */
	private static void getFlightPathData(FlightList<FlightPath> costMap, FlightList<FlightPath> timeMap) {
		try {
			//Opens the text file to read from
			File flightDataFile = new File("./src/FlightDataFile.txt");
			Scanner flightData = new Scanner(flightDataFile);

			int numFlightPlanDataPoints = 4;
			String dataSeparator = "|";
			String flightPathData;
			String[] data;
			StringTokenizer dataParse;

			flightData.nextLine(); //Skips first line because it has no use since the code is made to accept any amount of flight paths
			while (flightData.hasNextLine()) { //Reads the lines of flight path data
				flightPathData = flightData.nextLine();
				data = new String[numFlightPlanDataPoints];
				if (!flightPathData.isEmpty()) { //Ensures that if there are blank lines between data paths it doesn't exit the file and throw a NullPointerException
					dataParse = new StringTokenizer(flightPathData, dataSeparator); //Split each line to get the data
					int count = 0;
					while (dataParse.hasMoreElements() && count < numFlightPlanDataPoints) { //Read data into an array for access
						data[count] = dataParse.nextToken();
						count++;
					}
					//Add the flight paths from the .txt to the flight paths list
					costMap.add(new FlightPath(data[0], data[1], Integer.parseInt(data[2])));
					timeMap.add(new FlightPath(data[0], data[1], Integer.parseInt(data[3])));
				}
			}
			flightData.close();
		}
		catch(FileNotFoundException e){
			System.out.println("No File Found");
		}
	}

	/**
	 * Gets the .txt file of flight requests and prints out the shortest path between two cities based on the value type cost or time
	 * @param costMap : The map of all the flights based on cost
	 * @param timeMap : The map of all the flights based on time
	 */
	protected static void getRequestedFlights(FlightMap costMap, FlightMap timeMap){
		try {
			File flightRequestDataFile = new File("./src/RequestedFlightPlans.txt");
			Scanner requestedFlightData = new Scanner(flightRequestDataFile);

			String dataSeparator = "|";
			String requestedFlightPathData;
			StringTokenizer dataParse;
			String[] data;
			int numRequestedFlightPlanDataPoints = 3;
			int flightCount = 0;

			requestedFlightData.nextLine(); //Skips first line because it has no use
			while (requestedFlightData.hasNextLine()) {//Reads the lines of flight path data
				requestedFlightPathData = requestedFlightData.nextLine();
				data = new String[numRequestedFlightPlanDataPoints];
				if (!requestedFlightPathData.isEmpty()) { //Ensures that if there are blank lines between data paths it doesn't exit the file and throw a NullPointerException
					flightCount++;
					dataParse = new StringTokenizer(requestedFlightPathData, dataSeparator); //Split each line to get the data
					int count = 0;
					while (dataParse.hasMoreElements() && count < numRequestedFlightPlanDataPoints) { //Read data into an array for access
						data[count] = dataParse.nextToken();
						count++;
					}
				}

				System.out.print("Flight " + flightCount + ": " + data[0] + ", " + data[1]);
				switch (data[2]) {
					case "C": //Print out the flight path that has the smallest cost
						System.out.print(" (Cost)" + "\n");
						costMap.findShortestPath(data[0]);
						costMap.printPath(data[1]);
						System.out.print(". Cost: " + costMap.getTotal(data[1]) + "\n\n");
						break;
					case "T": //Print out the flight path that has the shortest time
						System.out.print(" (Time)" + "\n");
						timeMap.findShortestPath(data[0]);
						timeMap.printPath(data[1]);
						System.out.print(". Time: " + timeMap.getTotal(data[1]) + "\n\n");
						break;
					case "CT": //Print out the flight path that has the smallest cost and shortest time
						System.out.print(" (Cost & Time)" + "\n");
						costMap.findShortestPath(data[0]);
						costMap.printPath(data[1]);
						System.out.print(". Cost: " + costMap.getTotal(data[1]) + "\n");
						timeMap.findShortestPath(data[0]);
						timeMap.printPath(data[1]);
						System.out.print(". Time: " + timeMap.getTotal(data[1]) + "\n\n");
						break;
					default:
						System.err.print("Invalid Entry, must be \"T\", \"C\", or \"TC\"");
						break;
				}
			}
			requestedFlightData.close();
		}
		catch(FileNotFoundException e){
			System.out.println("No File Found");
		}
	}
}