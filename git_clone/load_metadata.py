from subprocess import Popen, PIPE
from datetime import date, timedelta, datetime
from dateutil.relativedelta import relativedelta
import github
from time import sleep
import pickle


SCRIPT_PATH = '/home/talkad/Downloads/thesis/data_gathering_script/git_clone/git_metadata.sh'
gh = github.Github("<gh_token>")

def get_user_info(username):
	'''
	Get user info from github
	'''
	try:
		gh_user = gh.search_users(username)[0]
	except: # rate limit exception
		sleep(60)
		return get_user_info(username)

	return gh_user.company, gh_user.location, gh_user.email

def extract_metadata(metadata):
	'''
	Extract 'PushedAt', 'Topics', 'Username' and 'Language'
	'''
	username_key, update_key, topic_key, lang_key = b'Owner:github.User{Login:', b'UpdatedAt:', b'Topics:', b'Language:'

	update_date = metadata[metadata.find(update_key) + len(update_key):]
	keywords = metadata[metadata.find(topic_key) + len(topic_key):]

	update_date = update_date[:update_date.find(b'}')].decode('utf-8')
	update_date = update_date[update_date.find('{')+1:]
	update_date = update_date[:update_date.find(' ')]

	update_date = datetime.strptime(update_date, '%Y-%m-%d').date()

	keywords = keywords[:keywords.find(b',')].decode('utf-8')
	keywords = list(map(lambda keyword: keyword.lower()[1:-1], keywords[1:-1].split(' ')))

	username = metadata[metadata.find(username_key) + len(username_key) + 1:]
	username = username[:username.find(b'\"')].decode('utf-8')

	lang = metadata[metadata.find(lang_key) + len(lang_key) + 1:]
	lang = lang[:lang.find(b'\"')].decode('utf-8')

	return username, update_date, keywords, lang


def load(start_date=None, end_date=None):
	'''
	Load repositories metadata - update_time and topics (keywords)

	Parameters:
		start_date - lower bound of dates
		end_date   - upper bound of dates
	'''
	month = 30
	max_results = 10**3
	start_date = date(2012, 1, 1) if start_date is None else start_date
	end_date = date.today() if end_date is None else end_date
	data = []

	while start_date <= end_date:
		start = start_date.strftime("%Y-%m-%d")
		delta = relativedelta(days=month)
		end = (start_date + delta).strftime("%Y-%m-%d")

		# exectue the "dry" version
		p = Popen([SCRIPT_PATH, start, end], stdin=PIPE, stdout=PIPE, stderr=PIPE)
		output, err = p.communicate()

		num_results = 0 if len(output) == 0 else len(output.split(b"github.Repository{"))

		if num_results < max_results:

			for metadata in output.split(b"github.Repository{"):
				if len(metadata) != 0:
					username, update_date, keywords, lang = extract_metadata(metadata)
					data += [{'username': username, 'update_date': update_date, 'keywords': keywords, 'lang':lang}]
			
			month = 30
			start_date += delta

		else:
			month = month // 2

	# save data as pickle
	with open('metadata.pickle', 'wb') as f:
            pickle.dump(data, f)


def get_info(metadata_path):
	'''
	Iterate over all users and save their email, company and location
	'''
	data = []

	with open(metadata_path, 'rb') as f:
		repo_lst = pickle.load(f)
		# print(len(repo_lst))

		for repo in repo_lst:
			company, location, email = get_user_info(repo['username'])
			data += [{'username': repo['username'], 'company': company, 'location': location, 'email': email}]

	with open('metadata_user.pickle', 'wb') as f:
		pickle.dump(data, f)



load()
# with open('metadata.pickle', 'rb') as f:
# 	lst = pickle.load(f)
# 	for l in lst:
# 		print(l)

# get_info('metadata.pickle')