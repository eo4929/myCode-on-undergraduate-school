package project13_1;

public class GraphObserver implements Observer {
	private NumberGenerator num;
	
	/* your code */
	public GraphObserver(NumberGenerator generator)
	{
		num = generator;
		num.addObserver(this);
	}
	@Override
	public void update(NumberGenerator generator) {
		// TODO Auto-generated method stub
		System.out.print("GraphObserver: ");
		
		for(int i=0; i<generator.getNumber(); i++) { System.out.print("*"); }
		System.out.println("");
		
		try {
			Thread.sleep(100);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
}
