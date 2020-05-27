package project13_1;

import java.util.ArrayList;
import java.util.Iterator;

public abstract class NumberGenerator {
	private ArrayList observers = new ArrayList();
	
	public abstract int getNumber();
	public abstract void execute();
	
	public void addObserver(Observer observer) {
		observers.add(observer);
	}
	
	public void deleteObserver(Observer observer) {
		observers.remove(observer);
	}
	
	public void notifyObservers() {
		/* your code */
		for(int i=0; i<observers.size(); i++){

			Observer observer = 
			(Observer)observers.get(i);

			observer.update(this);

		}
	}

}
