package project12_1;

public class ChocolateBoiler {
	private boolean empty;
	private boolean boiled;
	
	private ChocolateBoiler() {
		empty = true;
		boiled = false;
	}
	// Change the above code
	
	/* Add the code */
	private volatile static ChocolateBoiler uniqueInstance; //Ãß°¡ÇÔ
	
	public static ChocolateBoiler getInstance()
	{
		if(uniqueInstance == null) {
		synchronized(ChocolateBoiler.class)
		{
			if(uniqueInstance == null)
			{
				uniqueInstance = new ChocolateBoiler();
			}
		}
		}
		return uniqueInstance;
	}
	
	public void fill() {
		if(isEmpty()) {
			
			empty = false;
			boiled = false;
			System.out.println("Filling with mixture");
		}
		else
			System.out.println("Already filled");
	}
	
	public void drain() {
		if(!isEmpty() && isBoiled()) {
			empty = true;
			System.out.println("Draining the mixture");
		}
		else
			System.out.println("Already drained");
	}
	
	public void boil() {
		if(!isEmpty() && !isBoiled()) {
			boiled = true;
			System.out.println("Boiling the mixture");
		}
		else
			System.out.println("Already boiled");
	}
	
	public boolean isEmpty() {
		return empty;
	}
	
	public boolean isBoiled() {
		return boiled;
	}
}
