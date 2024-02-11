package SortingTechniques;

public class SelectionSort {
	public static void main(String[] args) {
		int[] arrayToSort = {64, 34, 25, 12, 22, 11, 90, 45, 72, 18};
		System.out.println("Original Array:");
		printArray(arrayToSort);
		// Call the selection sort function
		selectionSort(arrayToSort);
		System.out.println("\nSorted Array:");
		printArray(arrayToSort);
	}
		// Selection Sort function (incomplete - students need to complete the logic
		static void selectionSort(int[] arr) {
			int n = arr.length;
			// Implement Selection Sort algorithm here
			for (int i = 0; i < n - 1; i++) {
				//Checks for Minimum value to swap with current pointer
				int minIndex = i;
				for(int j = i +1; j < arr.length; j ++) {
					if(arr[j]<arr[minIndex]) {
						minIndex = j;
					}
				}
				swap(arr, i, minIndex);
			}
		}
		// Swap function
		static void swap(int[] arr, int a, int b) {
			int temp = arr[a];
			arr[a] = arr[b];
			arr[b] = temp;
		}
		// Display array function
		static void printArray(int[] arr) {
			for (int value : arr) {
				System.out.print(value + " ");
			}
			System.out.println();
		}
}
