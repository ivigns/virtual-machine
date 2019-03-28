var {
	zero 0
	one 1
	two 2
	tmp 0
}
const {
	enterNumber Which number of fibonacci sequence do you want:
	calculating Calculating...
	calculated The fibonacci number is 
	finished Programm finished
}
def {
	return1 {
		static {
			return 0
			n 0
		}
		pop &return
		pop &n
		push one
		push &return
		return
	}
	fibonacci {
		static {
			n 0
			result 0
			flag 0
			return 0
		}
		pop &return
		pop &n
		push &n
		push &return
		isequal zero &n
		pop &flag
		if &flag
		exec return1
		isequal one &n
		pop &flag
		if &flag
		exec return1

		sub one &n
		push &n
		call fibonacci

		pop &result
		pop &return
		pop &n
		push &result
		push &return

		sub two &n
		push &n
		call fibonacci

		pop &result
		pop &return
		pop tmp
		add tmp &result
		push &result
		push &return
		return
	}
	main {
		static {
			n 0
		}
		printconst enterNumber
		scan &n
		printconst calculating
		push &n
		call fibonacci
		pop &n
		printconst calculated
		print &n
		printconst finished
		exit
	}
}