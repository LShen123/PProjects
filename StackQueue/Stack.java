package StackQueue;
public class Stack {
	private int[] arr;
	private int top;
	private int capacity;
	public Stack(int size) {
		arr = new int[size];
		capacity = size;
		top = -1;
	}
	
	public void push(int x) {
		if(top == arr.length-1) {
			System.out.println("Stack Overflow");
			System.exit(1);
		}
		top = top +1;
		arr[top] = x;
	}
	public void printStack() {
		for(int i = 0; i <= top ; i ++) {
			System.out.print(arr[i] + " ");
		}
		System.out.println();
	}
	public int pop() {
		if(top == -1) {
			System.exit(1);
		}
		return arr[top--];
	}
	public int getSize() {
		return top+1;
	}
	public Boolean isEmpty() {
		return top == -1;
	}
	public Boolean isFull() {
		return top == capacity - 1;
	}
	public int peek() {
		return arr[top];
	}
}
