package project10_2;

public class Buffer {

	private int lock = 0;
	private double[] data;
	
	public Buffer(int size) {
		data = new double[size];
	}
	
	public int getSize() { return data.length; }
	
	public synchronized void add(double toAdd) throws InterruptedException {
		if(lock >= data.length) {
			System.out.println("Buffer is full");
			wait();
		}
		System.out.println("Adding item " + toAdd);
		System.out.flush();
		data[lock++] = toAdd;
		notifyAll();
	}
	
	public synchronized double remove() throws InterruptedException {
		if(lock <= 0) {
			System.out.println("Buffer is empty");
			wait();
		}
		double hold = data[--lock];
		data[lock] = 0.0;
		System.out.println("Removing item " + hold);
		System.out.flush();
		return hold;
	}
	
	public synchronized String toString() {
		String toReturn = "";
		for(int i=0; i<data.length; i++) {
			toReturn += String.format("%2.f", data[i]) + " ";
		}
		return toReturn;
	}
}
