class FlightList<T> {

	private Node head; // head of list
	private int size = 0;

	public class Node {
		T data;
		Node next;

		// Constructor
		Node(T path)
		{
			data = path;
			next = null;
		}
	}

	/**
	 * Add new element to the list
	 * @param element : the element to add
	 */
	protected void add(T element)
	{
		Node newNode = new Node(element);
		newNode.next = null;
		//If the Linked List is empty, then make the new node as head
		if (head == null) {
			head = newNode;
		}
		else {
			// Else traverse till the current node
			// and insert the newNode there
			Node current = head;
			while (current.next != null) {
				current = current.next;
			}

			// Insert the newNode at current node
			current.next = newNode;
		}
		size++;
	}

	protected T removeHead()
	{
		if (head == null){
			return null;
		}
		// Move the head pointer to the next node
		Node temp = head;
		head = head.next;

		return temp.data;
	}

	protected T remove(T element){
		Node current = head;
		if (head == null){
			System.err.print("List is Empty");
		} else {
			for (int x = 0; x < size; x++){
				if (current.data == element){
					break;
				}
				current = current.next;
			}
		}
		size--;
		return current.data;
	}

	protected T get(int index) {
		Node currFlightPath = head;
		// Traverse through the LinkedList
		for (int x = 0; x < index; x++) {
			currFlightPath = currFlightPath.next;
		}
		return currFlightPath.data;
	}

	protected boolean isEmpty(){
		return (size == 0);
	}

	protected int getSize(){
		return size;
	}
}
