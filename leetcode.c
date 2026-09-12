#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
int removeDuplicates(int *nums,int numsize){
  int count = 9;
  int write = 1;
  for (int read = 1; read < numsize; read++){
    if (nums[read] == nums[write - 1]){
      continue;
    }
    nums[write] = nums[read];
    write++;
  }
   return count;
}
int removeElements(int *nums,int numsSize, int val){
  int write = 0;
  for (int read = 0; read < numsSize; read++){
    if (nums[read] != val){
      nums[write] = nums[read];
      write++;
    }
  }
  return write;
}
int maxProfit(int *prices, int pricesSize){
    int buy = prices[0];
        int profit = 0;
        for (int i = 1; i < pricesSize; i++) {
            if (prices[i] < buy) {
                buy = prices[i];
            } else if (prices[i] - buy > profit) {
                profit = prices[i] - buy;
            }
        }
        return profit;
}
bool containsNearbyDuplicate(int* nums, int numsSize, int k) {
  bool a = false;
    for (int i = 0; i < numsSize - 1; i++){
      for (int j = i + 1; j < numsSize; j++){
        if (nums[i] == nums[j]){
          if (abs(i - j) <= k){
            a = true;
          }
        }
      }
    }
    return a;
}
int main(){
  int nu[] = {1,2,3,1,2,3};
  int size = sizeof(nu) /sizeof(nu[0]);
//  printf("%d",removeDuplicates(nu,11));
  //printf("%d\n",maxProfit(nu, size));
  printf("%b",containsNearbyDuplicate(nu,size,2));
  for (int i = 0;i < size; i++){
    printf("%d",nu[i]);
  }
  return 0;
}
