package project11_1;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;

public class GenericManager<T extends Comparable> /* your code */{
	private ArrayList<T> cList;

	public GenericManager() {
		/* your code */
		cList = new ArrayList<T>();
	}
	
	public void add(T a) {
		/* your code */
		cList.add(a);
	}
	
	public void sort() {
		Collections.sort(cList);
	}
	
	public String find(T a) {
		/* your code */
		
		String toReturn = "";
		
		for(Iterator<T>myIterator = cList.iterator();myIterator.hasNext();)
		{
			T listContact = myIterator.next();
			if(listContact.equals(a))
				toReturn += listContact.toString() +"\n"; 
		}
		
		
		return toReturn;
		 
	}
	
	public String toString() {
		/* your code */
		return cList.toString();
		/*
		for(String entry : cList)
			return "name : " + cLis
			*/
	}
}
