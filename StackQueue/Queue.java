package StackQueue;
import java.util.ArrayList;
public class Queue {
	private ArrayList<String> q = new ArrayList<String>();
	public void add(String s) {
		q.add(s);
	}
	public String pop() {
		String temp = q.get(0);
		q.remove(0);
		return temp;
	}
	public String toString() {
		String r = new String();
		r = r + "{";
		for(int i = 0; i < q.size(); i++) {
			r += q.get(i);
			if(i < q.size()-1) {
				r += ", ";
			}
		}
		return r + "}";
	}
}
