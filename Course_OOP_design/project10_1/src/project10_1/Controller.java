package project10_1;

public class Controller {
	public static void main(String[] args) {
		MyCustom custom = new MyCustom();
		Item[] itemList = new Item[5];
		
		System.out.println("Move Type : " + custom.getMoveType());
		System.out.println("Attack : " + custom.getIsAttack());
		System.out.println("ItemList : " + itemList.length);
		System.out.println("----------------------------------");
		
		custom.move("s");
		custom.attack("spacebar");
		
		itemList[0] = new Item("HP", "HP Desc", 50);
		itemList[1] = new Item("MP", "MP Desc", 30);
		itemList[2] = new Item("Food", "Food Desc", 100);
		itemList[3] = new Item("Key", "Key Desc", 10);
		itemList[4] = new Item("Gold", "Gold Desc", 1);
		
		System.out.println("Move Type : " + custom.getMoveType());
		System.out.println("Attack : " + custom.getIsAttack());
		System.out.println("ItemList");
		for(int i=0;i<itemList.length;i++) { System.out.println("Item[" + i + "] : " + itemList[i].getName() + " (" + itemList[i].getQuantity() + ")"); }
		System.out.println("----------------------------------");
		
		custom.sortItem(itemList);
		System.out.println("ItemList");
		for(int i=0;i<itemList.length;i++) { System.out.println("Item[" + i + "] : " + itemList[i].getName() + " (" + itemList[i].getQuantity() + ")"); }
		System.out.println("----------------------------------");
	}
}
