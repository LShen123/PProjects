package SortingTechniques;

public class QuickSort {
	public static void main(String[] args) {
		int[] arrayToSort = {64, 34, 25, 12, 22, 11, 90, 45, 72, 18};
		System.out.println("Original Array:");
		printArray(arrayToSort);
		// Call the quick sort function
		quickSort(arrayToSort, 0, arrayToSort.length - 1);
		System.out.println("\nSorted Array:");
		printArray(arrayToSort);
	}
		// Quick Sort function (incomplete - students need to complete the logic)
		static void quickSort(int[] arr, int low, int high) {
			// Implement Quick Sort algorithm here
			if (low < high) {
				//reIterates itself into smaller arrays to sort
				int pivotIndex = partition(arr,low,high);
				quickSort(arr, low, pivotIndex - 1);
				quickSort(arr,pivotIndex + 1, high);
			}
		}
		//Sorts the array around the last value
		static int partition(int[] arr, int low, int high) {
			int pivot = arr[high];
			int i = low - 1;
			for(int j = low; j <= high-1; j ++) {
				if(arr[j] < pivot) {
					i = i + 1;
					swap(arr, i, j);
				}
			}
			swap(arr, i+1, high);
			return i + 1;
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
