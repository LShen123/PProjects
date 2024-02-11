package SortingTechniques;

public class BubbleSort {
	public static void main(String[] args) {
		int[] arrayToSort = {64, 34, 25, 12, 22, 11, 90, 45, 72, 18};
		System.out.println("Original Array:");
		printArray(arrayToSort);
		// Call the bubble sort function
		bubbleSort(arrayToSort);
		System.out.println("\nSorted Array:");
		printArray(arrayToSort);
	}
		// Bubble Sort function (incomplete - students need to complete the logic
		static void bubbleSort(int[] arr) {
			int n = arr.length;
			// Implement Bubble Sort algorithm here
			for (int i = 0; i < n - 1; i++) {
				for (int j = 0; j < n - i - 1; j++) {
					//Checks if value is greater than the value after and switches accordingly
					if(arr[j] > arr[j+1]) {
						int temp = arr[j+1];
						arr[j+1] = arr[j];
						arr[j] = temp; 
					}
				}
			}
		}
		// Display array function
		static void printArray(int[] arr) {
			for (int value : arr) {
				System.out.print(value + " ");
			}
			System.out.println();
		}
}
