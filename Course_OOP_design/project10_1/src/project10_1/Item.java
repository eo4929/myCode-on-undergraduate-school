package project10_1;

public class Item implements Comparable {
	private String name;
	private String desc;
	private int quantity;
	
	public Item(String name, String desc, int quantity) {
		this.name = name;
		this.desc = desc;
		this.quantity = quantity;
	}
	
	public String getName() { return this.name; }
	public String getDesc() { return this.desc; }
	public int getQuantity() { return this.quantity; }
	
	public void setName(String name) { this.name = name; }
	public void setDesc(String desc) { this.desc = desc; }
	public void setQuantity(int quantity) { this.quantity = quantity; }
	public int compareTo(Object compareItem) {
		/* your code */
		if(compareItem == null)
		{
			System.out.println("null error!\n");
			return -1;
		}
		
		if(!this.getClass().equals(compareItem.getClass()))
		{
			System.out.println("getclass error!\n");
			return -1;
		}
		
		Item toCompare = (Item)compareItem;
		
		if(this.quantity < toCompare.quantity)
			return -1;
		else if(this.quantity == toCompare.quantity)
			return 0;
		else
			return 1;
	}
}
