// https://github.com/PierreBoyeau/levenshtein_distance

#include<string>
#include<algorithm>
#include<iostream>
using namespace std;


int mini(int a, int b, int c){
    return(min(a, min(b,c)));
}

/* void find_path(int& i, int& j, int* dist, int size2, string word1, string word2){
    ///
    /// Algorithm that gives the sequence of operations
    /// (that can have no cost when one character is substituted by itself)
    /// to go from one word to the other.
    ///
    if(i>0 && j>0){
        int value_mini = mini(dist[(size2+1)*(i-1)+j-1], dist[(size2+1)*(i)+j-1],
                dist[(size2+1)*(i-1)+j]);
        if(dist[(size2+1)*(i-1)+j-1]==value_mini){ //substitution of 2 characters
            cout<<word1[i-1]<<" exchanged with "<<word2[j-1]<<endl;
            i--; j--;
        }
        else if(dist[(size2+1)*(i)+j-1]==value_mini){ // addition of a characters
            cout<<"add "<<word2[j-1]<<endl;
            j--;
        }
        else{  // suppression of a character
            cout<<"suppress "<<word1[i-1]<<endl;
            i--;
        }
    }
    else if(i==0 && j>0){
        cout<<"add "<<word2[j-1]<<endl; j--;
    }
    else if(j==0 && i>0){
        cout<<"suppress "<<word1[i]<<endl; i--;}

} */

int levenshtein_dist(const string &word1, const string &word2){
    ///
    ///  Please use lower-case strings
    /// word1 : first word
    /// word2 : second word
    ///
	
    int size1 = word1.size(), size2 = word2.size();
    int suppr_dist, insert_dist, subs_dist;
    int* dist = new int[(size1+1)*(size2+1)];

    for(int i=0; i<size1+1; ++i)
        dist[(size2+1)*i] = i;
    for(int j=0; j<size2+1; ++j)
        dist[j] = j;
    for(int i=1; i<size1+1; ++i){
        for(int j=1; j<size2+1; ++j){
            suppr_dist = dist[(size2+1)*(i-1)+j] + 1;
            insert_dist = dist[(size2+1)*i+j-1] + 1;
            subs_dist = dist[(size2+1)*(i-1)+j-1];
            if(word1[i-1]!=word2[j-1]){  // word indexes are implemented differently.
                subs_dist += 1;
            }
            dist[(size2+1)*i+j] = mini(suppr_dist, insert_dist, subs_dist);
        }
    }
    // --------------------------------------------------------
    int res = dist[(size1+1)*(size2+1) - 1];
    delete dist;
    return(res);
}



int dl_dist(const string &word1, const string &word2){
    /// Damerau-Levenshtein distance
    ///  Please use lower-case strings
    /// word1 : first word
    /// word2 : second word
    ///
    int size1 = word1.size(), size2 = word2.size();
    int suppr_dist, insert_dist, subs_dist, val;
    int* dist = new int[(size1+1)*(size2+1)];

    for(int i=0; i<size1+1; ++i)
        dist[(size2+1)*i] = i;
    for(int j=0; j<size2+1; ++j)
        dist[j] = j;
    for(int i=1; i<size1+1; ++i){
        for(int j=1; j<size2+1; ++j){
            suppr_dist = dist[(size2+1)*(i-1)+j] + 1;
            insert_dist = dist[(size2+1)*i+j-1] + 1;
            subs_dist = dist[(size2+1)*(i-1)+j-1];
            if(word1[i-1]!=word2[j-1])  // word indexes are implemented differently.
                subs_dist += 1;
            val = mini(suppr_dist, insert_dist, subs_dist);
            if(((i>=2) && (j>=2)) && ((word1[i-1]==word2[j-2]) && (word1[i-2]==word2[j-1])))
                val = min(dist[(size2+1)*(i-2)+j-2]+1, val);
            dist[(size2+1)*i+j] = val;
        }
    }

    int res = dist[(size1+1)*(size2+1) - 1];
    delete dist;
    return(res);
}

// L. Schiffmann, 2021
double levenshtein_distp(const string &str1, const string &str2) {
  int l = levenshtein_dist(str1,str2);
  int m = str1.size();
  int n = str2.size();  
  if (max(m,n) == 0) {
	 return 0;
  }  
  return 1.0 - (double)l / max(m,n);
}

// L. Schiffmann, 2021
double dl_distp(const string &str1, const string &str2) {	
  int l = dl_dist(str1,str2);
  int m = str1.size();
  int n = str2.size();  
  if (max(m,n) == 0) {
	 return 0;
  }
  return 1.0 - (double)l / max(m,n);
}


