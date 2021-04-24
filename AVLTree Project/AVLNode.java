public class AVLNode implements TreePrinter.PrintableNode {
	private String ISBN; //(ISBN number)
	private Book book; //create a class representing a book with minimum attributes
	int height, balance;
	AVLNode leftPtr, rightPtr;

	protected AVLNode(String ISBN, Book book){
		this.ISBN = ISBN;
		this.book = book;
	}

	@Override
	public String getText() {
		return ISBN;
	}

	@Override
	public AVLNode getLeft() {
		return leftPtr;
	}

	@Override
	public AVLNode getRight() {
		return rightPtr;
	}

	public String toString(){
		return "ISBN: " + ISBN + "\n" + book.toString();
	}
}
