package project13_2;

public class Cinnamon extends CondimentDecorator/* your code */{
	/* your code */
	Beverage beverage;

	public Cinnamon(Beverage beverage)
	{
		this.beverage = beverage;
	}

	public String getDescription()
	{
		return beverage.getDescription()
		+ ", Cinnamon";
	}

	public double cost()
	{
		return beverage.cost() + 0.5;
	}
}
