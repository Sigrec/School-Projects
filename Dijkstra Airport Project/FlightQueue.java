import java.util.Comparator;

class FlightQueue<T extends Comparable> {

	private FlightList<T> list = new FlightList<>();

	public void enqueue(T item){
		list.add(item);
	}

	public T dequeue(){
		return list.removeHead();
	}

	public void dequeue(T item){
		int maxIndex = 0;
		for (int x = 1; x < list.getSize(); x++){
			if (list.get(x).compareTo(item) >= 0){
				maxIndex = 0;
				list.remove(item);
			}
		}
	}

	public boolean isQueueEmpty(){
		return list.isEmpty();
	}

	public int size(){
		return list.getSize();
	}

	public T get(int index){
		return list.get(index);
	}
}
 