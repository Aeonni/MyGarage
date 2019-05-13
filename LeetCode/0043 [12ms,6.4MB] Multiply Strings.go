/*
Aeonni 2019.05
	Runtime:  12ms, faster than 22.82%
    Memory Usage:  6.4MB, less than 22.22%
*/

/*
Example 1:

Input: num1 = "2", num2 = "3"
Output: "6"
Example 2:

Input: num1 = "123", num2 = "456"
Output: "56088"
Note:

The length of both num1 and num2 is < 110.
Both num1 and num2 contain only digits 0-9.
Both num1 and num2 do not contain any leading zero, except the number 0 itself.
You must not use any built-in BigInteger library or convert the inputs to integer directly.
*/

func add(num1 string, num2 string) string {
	l1, l2 := len(num1), len(num2)
	n1, n2 := []byte(num1), []byte(num2)
	if l1 < l2 {
		n1, n2 = []byte(num2), []byte(num1)
		l1, l2 = l2, l1
	}
	for i := -1; i+l2 >= 0; i-- {
		n1[l1+i] += n2[l2+i] - 48
	}
	var flag byte = 0
	for i := -1; i+l1 >= 0; i-- {
		n1[l1+i] += flag
		flag = 0
		if n1[l1+i] > 57 {
			flag = 1
			n1[l1+i] -= 10
		}
	}
	if flag > 0 {
		return "1" + string(n1)
	}
	return string(n1)
}

func multiply_single(num1 string, num2 uint8) string {
	l1 := len(num1)
	n1 := []byte(num1)
	for i := -1; i+l1 >= 0; i-- {
		n1[l1+i] -= '0'
		n1[l1+i] *= num2
	}
	var flag byte = 0
	for i := -1; i+l1 >= 0; i-- {
		n1[l1+i] += flag
		flag = 0
		if n1[l1+i] >= 10 {
			flag = n1[l1+i] / 10
			n1[l1+i] = n1[l1+i] % 10
		}
		n1[l1+i] += '0'
	}
	if flag > 0 {
		return string(flag+'0') + string(n1)
	}
	return string(n1)
}

func multiply(num1 string, num2 string) string {
	l2 := len(num2)

	result := "0"
	r := ""
	for i := -1; i+l2 >= 0; i-- {
		r = multiply_single(num1, num2[i+l2]-'0')
		for j := 0; j < -i-1; j++ {
			r += "0"
		}
		result = add(result, r)
	}

	for result[0] == '0' && len(result) > 1 {
		result = result[1:]
	}
	return result
}