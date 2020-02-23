package project10_1;

import java.util.Arrays;

public class MyCustom {
	private Integer move_type = 0;
	private Boolean isAttack = false;
	
	public void move(String key) {
		/* your code */
		if(key == "w")
			this.move_type = 1;
		else if(key == "s")
			this.move_type = 2;
		else if(key == "a")
			this.move_type = 3;
		else if(key == "d")
			this.move_type = 4;
		else
			this.move_type = 5;
	}
	public void attack(String key) {
		/* your code */
		if(key == "spacebar")
			this.isAttack = true;
		else
			this.isAttack = false;
	}
	public void sortItem(Item[] itemList) {
		/* your code */
		Arrays.sort(itemList);
	}
	public Integer getMoveType() {
		/* your code */
		return this.move_type;
	}
	public Boolean getIsAttack() {
		/* your code */
		return this.isAttack;
	}
}