public class Book {
	private String author;
	private String title;
	private String genre;

	protected Book(String author, String title, String genre) {
		this.author = author;
		this.title = title;
		this.genre = genre;
	}

	public String toString(){
		return "Author: " + author + "\nTitle: " + title + "\nCategory: " + genre + "\n";
	}
}

