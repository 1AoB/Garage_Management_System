int bubble_sort(int arr[],int len){
	for(int i = 0 ; i < len-1 ;i++)
	{
		for(int j = 0; j<len-i-1 ; j++){
			if(arr[j]>arr[j+1]){
				int tmp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = tmp;
			}
		}
	}
	return 0;
}
