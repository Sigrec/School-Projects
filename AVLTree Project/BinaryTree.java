import java.util.Random;
import java.util.Scanner;

public class BinaryTree {
	private static Node root = null;

	static class Node implements TreePrinter.PrintableNode {
		Node right, left;
		int key, height, balance;

		private Node(int key) {
			this.key = key;
		}

		@Override
		public String getText() {
			return Integer.toString(key);
		}

		@Override
		public Node getLeft() {
			return left;
		}

		@Override
		public Node getRight() {
			return right;
		}

		public String toString() {
			return "Key: " + key;
		}
	}

	//Displays the tree
	private void displayTree() {
		System.out.println(TreePrinter.getTreeDisplay(root));
	}

	/**
	 * Checks to see if the AVL tree already has the node and if it exists, then inserts
	 * @param newNode, the node that is being inserted
	 */
	private static void insert(Node newNode) {
		root = insert(root, newNode);
	}

	/**
	 * Updates the nodes height and balance
	 * @param node, the node being updated
	 */
	private static void updateNode(Node node){
		//Get the heights of the nodes children & update the nodes height
		int leftNodeHeight = ((node.left == null) ? -1 : node.left.height);
		int rightNodeHeight = ((node.right == null) ? -1 : node.right.height);
		node.height = 1 + Math.max(leftNodeHeight, rightNodeHeight); //Update the height of the node

		//Update the balance of the node
		node.balance = leftNodeHeight - rightNodeHeight;
		//System.out.println("Balance: " + node.balance); System.out.println("Height: " + node.height);
	}

	/**
	 * Inserts the Node based on its key
	 * @param node, the root node
	 * @param newNode, the node you are trying to insert
	 * @return Node, the updated node that is being inserted
	 */
	private static Node insert(Node node, Node newNode) {
		if (node == null) {
			return newNode;
		}
		Random rand = new Random();
		int num = rand.nextInt(2);
		//System.out.println("Val :" + num);
		if (num == 0) { //If 0 insert in the left subtree
			node.left = insert(node.left, newNode);
		}
		else { //If 1 insert in the right subtree
			node.right = insert(node.right, newNode);
		}
		updateNode(node);
		return node;
	}

	/**
	 * Traverses the tree to see whether it follows the BST structure and order
	 * @param node, node being checked
	 * @param min, check to see if the left node of the node being checked, is smaller than node being checked
	 * @param max, check to see if the right node of the node being checked, is smaller than node being checked
	 * @return bool, whether or not the tree is a BST or not
	 */
	private static boolean isBST(Node node, int min, int max) {
		if (node == null){
			return true;
		}
		if (node.left != null && node.key < node.left.key){
			return false;
		}
		else if (node.right != null && node.key > node.right.key){
			return false;
		}
		return isBST(node.left, min, node.key) && isBST(node.right, node.key, max);
	}

	/**
	 * Traverses the tree to anc checks each nodes balances to determine whether it's an AVL tree (method is only called if isBST method is true)
	 * @param node, the node being checked
	 * @return bool, whether or not the tree is a AVL tree or not
	 */
	private static boolean isAVL(Node node) {
		if (node == null){
			return true;
		}
		int balanceFactor = node.balance;
		//System.out.println("Balance: " + balanceFactor);
		if (balanceFactor >= 2 || balanceFactor <= -2){
			return false;
		}
		return isAVL(node.left) && isAVL(node.right);
	}

	/**
	 * Traverses through the tree to find the Node that is being searched for
	 * @param node, the root node
	 * @param newNode, the node you are looking for
	 * @return AVLNode, the node that is being looked for
	 */
	private static Node get(Node node, Node newNode) {
		if (node == null){
			return null;
		}
		int cmp = newNode.getText().compareTo(node.getText()); //Compares 2 nodes to determine whether to check right or left
		if (cmp < 0){ //Check if new node is smaller than current, if so check left subtree
			return get(node.left, newNode);
		}
		else if (cmp > 0){ //Check if new node is smaller than current, if so check right subtree
			return get(node.right, newNode);
		}
		return node;
	}

	/**
	 * Calls the get method to search for a node, and if it's not null, returns whether it was found or not
	 * @param newNode, the node that is being searched for
	 * @return bool, whether or not the node was found in the AVL tree
	 */
	private static boolean contains(Node newNode) {
		return get(root, newNode) != null;
	}

	public static void main(String[] arg){
		BinaryTree tree = new BinaryTree();

		//Ask user for the save of the tree
		System.out.print("Enter Tree Size: ");
		Scanner input = new Scanner(System.in);
		int treeSize = input.nextInt();

		//Generate random numbers to be added to tree
		Random ranNum = new Random();
		Node newNode;
		int count = 0;
		while (count < treeSize){
			newNode = new Node(ranNum.nextInt(treeSize*10));
			if (!contains(newNode) && newNode != null){
				insert(newNode);
				count++;
			}
		}
		//System.out.println("The tree is a AVL Tree: " + isAVL());
		if (isBST(root, Integer.MIN_VALUE, Integer.MAX_VALUE)){ //If tree is BST
			if (isAVL(root)){ //If tree is an AVL
				System.out.println("The tree is a BST & AVL Tree");
			}
			else{
				System.out.println("The tree is a BST Tree but not a AVL Tree");
			}
		}
		else{
			System.out.println("The tree is not a BST or AVL Tree");
		}
		tree.displayTree();
	}
}