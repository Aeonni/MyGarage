/*
Aeonni 2019.05
	Runtime: 4 ms, faster than 99.36%
    Memory Usage: 4.1 MB, less than 89.09%
*/

/*
The string "PAYPALISHIRING" is written in a zigzag pattern on a given number of rows like this: (you may want to display this pattern in a fixed font for better legibility)

P   A   H   N
A P L S I I G
Y   I   R
And then read line by line: "PAHNAPLSIIGYIR"

Write the code that will take a string and make this conversion given a number of rows:

string convert(string s, int numRows);
Example 1:

Input: s = "PAYPALISHIRING", numRows = 3
Output: "PAHNAPLSIIGYIR"
Example 2:

Input: s = "PAYPALISHIRING", numRows = 4
Output: "PINALSIGYAHRPI"
Explanation:

P     I    N
A   L S  I G
Y A   H R
P     I
*/

func convert(s string, numRows int) string {
	if numRows == 1 {
		return s
	}

	flag := 0
	ret := []byte{}
	for line := 0; line < numRows; line++ {
		for i := line; i < len(s); {
			ret = append(ret, s[i])

			//             if i == line {
			//                 fmt.Printf("%c",s[i])
			//             }else if(flag == 1 || line == 0) && line != n-1 {
			//                 fmt.Printf(fmt.Sprintf("%%%dc", n-line-1),s[i])
			//             }else {
			//                 fmt.Printf(fmt.Sprintf("%%%dc", line),s[i])
			//             }

			if (flag == 0 || line == 0) && line != numRows-1 {
				i += 2*(numRows-line) - 2
				flag = 1
			} else {
				i += 2 * line
				flag = 0
			}
		}
		flag = 0
		//         fmt.Println("")
	}
	return string(ret)
}