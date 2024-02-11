package SortingTechniques;

public class InsertionSort {
	public static void main(String[] args) {
		int[] arrayToSort = {64, 34, 25, 12, 22, 11, 90, 45, 72, 18};
		System.out.println("Original Array:");
		printArray(arrayToSort);
		// Call the insertion sort function
		insertionSort(arrayToSort);
		System.out.println("\nSorted Array:");
		printArray(arrayToSort);
	}
		// Insertion Sort function (incomplete - students need to complete the logic
		static void insertionSort(int[] arr) {
			int n = arr.length;
			// Implement Insertion Sort algorithm here
			for (int i = 1; i < n; ++i) {
				//Creates key to see if values before are greater or less than the key
				int key = arr[i];
				int j = i - 1;
				//Moves values greater than key forward and changes key to that value
				while(j >= 0 && arr[j]>key){
					arr[j+1] = arr[j];
					j = j-1;
					printArray(arr);
				}
				//Places key back into the array
				arr[j+1] = key;
				printArray(arr);
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
