package StackQueue;
public class BalancedP {
	public static boolean isBalanced(String p) {
		Stack s = new Stack(100);
		for(int i = 0; i < p.length(); i ++) {
			if(p.substring(i,i+1).equals("(")) {
				s.push(1);
			}
			else if(p.substring(i,i+1).equals(")")) {
				if(s.isEmpty()) {
					return false;
				}
				s.pop();
			}
		}
		if(s.isEmpty()) {
			return true;
		}
		else {
			return false;
		}
	}
	public static void main(String[] args) {
		String p = "(()((())()))";
		p = p.replaceAll("\\s+","");
		System.out.print("The parentheses " + p + " is ");
		if(isBalanced(p)) {
			System.out.println("Balanced");
		}
		else {
			System.out.println("Unbalanced");
		}
	}
}
