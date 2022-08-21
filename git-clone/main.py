from subprocess import Popen, PIPE
from datetime import date, timedelta
from dateutil.relativedelta import relativedelta

month = 30
max_results = 10**3
start_date = date(2008, 1, 1)
end_date = date.today()

total_repos = 0


while start_date <= end_date:
	start = start_date.strftime("%Y-%m-%d")
	delta = relativedelta(days=month)
	end = (start_date + delta).strftime("%Y-%m-%d")

	# exectue the "dry" version
	p = Popen(["./git_clone.sh", "y", start, end], stdin=PIPE, stdout=PIPE, stderr=PIPE)
	output, err = p.communicate()

	num_results = len(output.split(b"\ndry-run:"))

	if num_results < max_results:
		#clone
		p = Popen(["./git_clone.sh", "", start, end], stdin=PIPE, stdout=PIPE, stderr=PIPE)
		output, err = p.communicate()
		print(f'cloned {num_results} repos from {start} to {end}')
		
		month = 30
		total_repos += num_results
		start_date += delta
	else:
		month = month // 2



print(f'total number of repos cloned: {total_repos}')		
	
