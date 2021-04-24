import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class AVLTree{
	private static AVLNode root = null;
	private int treeSize = 0;
	private ArrayList<String> imbalanceOutputs = new ArrayList<>();

	public static void main(String[] args) throws FileNotFoundException{
		//Create the AVL Tree
		AVLTree tree = new AVLTree();

		//Get the file name inputted by the user
		System.out.print("Enter FileName With Document Type Prefix (txt): ");
		Scanner fileNameInput = new Scanner(System.in);
		String fileName = fileNameInput.next();
		File bookList = new File("./src/" + fileName);

		//If the file exists get the data
		if (bookList.exists()) {
			AVLNode avlObj;
			String[] bookData;
			String bookInfo, regex; regex = "\\*"; //The character used to separate a book objects data

			//Open the file and check if the file has valid lines to read
			Scanner data = new Scanner(bookList);
			while (data.hasNextLine()) {
				bookInfo = data.nextLine();
				if (!bookInfo.isEmpty()){ //Checks to see if the line it's reading is empty
					bookData = bookInfo.split(regex); //Parse the book data based on some string, can't use space because title and author can have spaces
					avlObj = new AVLNode(bookData[0], new Book(bookData[1], bookData[2], bookData[3]));
					tree.insertByISBN(avlObj);
				}
				bookData = null; //Clear list before reading in next book
			}
		}
		tree.display();
	}

	/**
	 * Rotate a node right
	 * @param troubleNode , the node that has a height <= -2
	 * @return AVLNode, the new parent node after a right rotation
	 */
	private AVLNode rotateRight(AVLNode troubleNode){
		AVLNode newParent = troubleNode.leftPtr; //Initializes the trouble nodes new parent as its left node
		troubleNode.leftPtr = newParent.rightPtr; //Sets the trouble nodes left ptr to the new parents left ptr since the trouble node will become its right node
		newParent.rightPtr = troubleNode; //Sets the right node of the new parent to the trouble node
		updateNode(troubleNode);
		updateNode(newParent);
		return newParent;
	}

	/**
	 * Rotate a node left
	 * @param troubleNode, the node that has a height >= 2
	 * @return AVLNode, the new parent node after a left rotation
	 */
	private AVLNode rotateLeft(AVLNode troubleNode){
		AVLNode newParent = troubleNode.rightPtr; //Initializes the trouble nodes new parent as its right node
		troubleNode.rightPtr = newParent.leftPtr; //Sets the trouble nodes right ptr to the new parents left ptr since the trouble node will become its right node
		newParent.leftPtr = troubleNode; //Sets the left node of the new parent to the trouble node
		updateNode(troubleNode);
		updateNode(newParent);
		return newParent;
	}

	/**
	 * Updates the nodes height and balance
	 * @param node, the node being updated
	 */
	private void updateNode(AVLNode node){
		//Get the heights of the nodes children
		int leftNodeHeight = ((node.leftPtr == null) ? -1 : node.leftPtr.height);
		int rightNodeHeight = ((node.rightPtr == null) ? -1 : node.rightPtr.height);
		node.height = 1 + Math.max(leftNodeHeight, rightNodeHeight); //Update the height of the node

		//Update the balance of the node
		node.balance = leftNodeHeight - rightNodeHeight;
	}

	/**
	 * Balances the AVL Tree at the trouble node
	 * @param node, the trouble node that needs to be "fixed" based on its balance
	 * @return AVLNode, recursive return on the trouble node -> the updated trouble node
	 */
	private AVLNode balance(AVLNode node){
		int nodeBL = node.balance;
		if (nodeBL == 2){ //Left Heavy Subtree
			if (node.leftPtr.balance >= 0){ //Left-Left Case
				imbalanceOutputs.add(node.leftPtr.rightPtr.getText() + "; fixed in Left-Left Rotation");
				return rotateRight(node);
			}
			else{ //Left-Right Case
				imbalanceOutputs.add(node.leftPtr.leftPtr.getText() + "; fixed in Left-Right Rotation");
				node.leftPtr = rotateLeft(node.leftPtr);
				return rotateRight(node);
			}
		}
		else if (nodeBL == -2){ //Right Heavy Subtree
			if (node.rightPtr.balance <= 0){//Right-Right Case
				imbalanceOutputs.add(node.rightPtr.rightPtr.getText() + "; fixed in Right-Right Rotation");
				return rotateLeft(node);
			}
			else{ //Right-Left Case
				imbalanceOutputs.add(node.rightPtr.leftPtr.getText() + "; fixed in Right-Left Rotation");
				node.rightPtr = rotateRight(node.rightPtr);
				return rotateLeft(node);
			}
		}
		return node;
	}

	/**
	 * Checks to see if the AVL tree already has the node and if it exists, then inserts
	 * @param newNode, the node that is being inserted
	 */
	private void insertByISBN(AVLNode newNode) {
		if (!contains(newNode) && newNode != null) {
			root = insertByISBN(root, newNode);
			treeSize++;
		}
	}

	/**
	 * Inserts the AVLNode by its ISBN number
	 * @param node, the root node
	 * @param newNode, the node you are trying to insert
	 * @return AVLNode, the updated node that is being inserted
	 */
	private AVLNode insertByISBN(AVLNode node, AVLNode newNode){
		if(node == null){
			return newNode;
		}
		int compareValue = newNode.getText().compareTo(node.getText());
		if (compareValue < 0){ //Check if new node is smaller than current & insert left
			node.leftPtr = insertByISBN(node.leftPtr, newNode);
		}
		else{ //Check if new node is greater than current & insert right
			node.rightPtr = insertByISBN(node.rightPtr, newNode);
		}
		updateNode(node);
		return balance(node);
	}

	/**
	 * Traverses through the tree to find the Node that is being searched for
	 * @param node, the root node
	 * @param newNode, the node you are looking for
	 * @return AVLNode, the node that is being looked for
	 */
	private static AVLNode get(AVLNode node, AVLNode newNode) {
		if (node == null){
			return null;
		}
		int cmp = newNode.getText().compareTo(node.getText()); //Compares 2 nodes to determine whether to check right or left
		if (cmp < 0){ //Check if new node is smaller than current, if so check left subtree
			return get(node.leftPtr, newNode);
		}
		else if (cmp > 0){ //Check if new node is smaller than current, if so check right subtree
			return get(node.rightPtr, newNode);
		}
		return node;
	}

	/**
	 * Calls the get method to search for a node, and if it's not null, returns whether it was found or not
	 * @param newNode, the node that is being searched for
	 * @return bool, whether or not the node was found in the AVL tree
	 */
	private static boolean contains(AVLNode newNode) {
		return get(root, newNode) != null;
	}

	//Displays the AVL tree and prints where imbalances occurred and what case was used
	private void display(){
		System.out.println(TreePrinter.getTreeDisplay(root));
		for (String output : imbalanceOutputs){
			System.out.println("Imbalance occurred at inserting ISBN " + output);
		}
	}
}
