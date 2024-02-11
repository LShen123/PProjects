package StackQueue;
import java.util.*;
public class HotPotato {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Queue queue = new Queue();
		queue.add("Brad");
		queue.add("Kent");
		queue.add("Jane");
		queue.add("Susan");
		queue.add("David");
		queue.add("Bill");
		Scanner s = new Scanner(System.in);
		System.out.println("Choose a Constant: ");
		int musical_num = s.nextInt();
		s.close();
		for(int i = 0; i < 5; i ++) {
			for(int j = 0; j < musical_num; j ++) {
				queue.add(queue.pop());
			}
			System.out.println(queue.pop());
			System.out.println(queue);
		}
	}

}
