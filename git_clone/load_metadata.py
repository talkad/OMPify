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
					print(metadata)
					break
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
			print(f'repo name: {repo}')
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


# b' Owner:github.User{Login:"twiecki", ID:674200, NodeID:"MDQ6VXNlcjY3NDIwMA==", AvatarURL:"https://avatars.githubusercontent.com/u/674200?v=4", HTMLURL:"https://github.com/twiecki", GravatarID:"", Type:"User", SiteAdmin:false, URL:"https://api.github.com/users/twiecki", EventsURL:"https://api.github.com/users/twiecki/events{/privacy}", FollowingURL:"https://api.github.com/users/twiecki/following{/other_user}", FollowersURL:"https://api.github.com/users/twiecki/followers", GistsURL:"https://api.github.com/users/twiecki/gists{/gist_id}", OrganizationsURL:"https://api.github.com/users/twiecki/orgs", ReceivedEventsURL:"https://api.github.com/users/twiecki/received_events", ReposURL:"https://api.github.com/users/twiecki/repos", StarredURL:"https://api.github.com/users/twiecki/starred{/owner}{/repo}", SubscriptionsURL:"https://api.github.com/users/twiecki/subscriptions"}, Name:"mpi4py_map", FullName:"twiecki/mpi4py_map", Description:"mpi4py_map provides a simple map() interface to mpi4py that allows easy parallelization of function evaluations over sequential input.", Homepage:"", DefaultBranch:"master", CreatedAt:github.Timestamp{2012-01-09 16:32:32 +0000 UTC}, PushedAt:github.Timestamp{2015-03-06 11:08:16 +0000 UTC}, UpdatedAt:github.Timestamp{2022-01-13 22:44:12 +0000 UTC}, HTMLURL:"https://github.com/twiecki/mpi4py_map", CloneURL:"https://github.com/twiecki/mpi4py_map.git", GitURL:"git://github.com/twiecki/mpi4py_map.git", SSHURL:"git@github.com:twiecki/mpi4py_map.git", SVNURL:"https://github.com/twiecki/mpi4py_map", Language:"Python", Fork:false, ForksCount:10, OpenIssuesCount:0, StargazersCount:24, WatchersCount:24, Size:157, Permissions:map[admin:false maintain:false pull:true push:false triage:false], Topics:[], Private:false, HasIssues:true, HasWiki:true, HasPages:false, HasProjects:true, HasDownloads:true, Archived:false, URL:"https://api.github.com/repos/twiecki/mpi4py_map", ArchiveURL:"https://api.github.com/repos/twiecki/mpi4py_map/{archive_format}{/ref}", AssigneesURL:"https://api.github.com/repos/twiecki/mpi4py_map/assignees{/user}", BlobsURL:"https://api.github.com/repos/twiecki/mpi4py_map/git/blobs{/sha}", BranchesURL:"https://api.github.com/repos/twiecki/mpi4py_map/branches{/branch}", CollaboratorsURL:"https://api.github.com/repos/twiecki/mpi4py_map/collaborators{/collaborator}", CommentsURL:"https://api.github.com/repos/twiecki/mpi4py_map/comments{/number}", CommitsURL:"https://api.github.com/repos/twiecki/mpi4py_map/commits{/sha}", CompareURL:"https://api.github.com/repos/twiecki/mpi4py_map/compare/{base}...{head}", ContentsURL:"https://api.github.com/repos/twiecki/mpi4py_map/contents/{+path}", ContributorsURL:"https://api.github.com/repos/twiecki/mpi4py_map/contributors", DeploymentsURL:"https://api.github.com/repos/twiecki/mpi4py_map/deployments", DownloadsURL:"https://api.github.com/repos/twiecki/mpi4py_map/downloads", EventsURL:"https://api.github.com/repos/twiecki/mpi4py_map/events", ForksURL:"https://api.github.com/repos/twiecki/mpi4py_map/forks", GitCommitsURL:"https://api.github.com/repos/twiecki/mpi4py_map/git/commits{/sha}", GitRefsURL:"https://api.github.com/repos/twiecki/mpi4py_map/git/refs{/sha}", GitTagsURL:"https://api.github.com/repos/twiecki/mpi4py_map/git/tags{/sha}", HooksURL:"https://api.github.com/repos/twiecki/mpi4py_map/hooks", IssueCommentURL:"https://api.github.com/repos/twiecki/mpi4py_map/issues/comments{/number}", IssueEventsURL:"https://api.github.com/repos/twiecki/mpi4py_map/issues/events{/number}", IssuesURL:"https://api.github.com/repos/twiecki/mpi4py_map/issues{/number}", KeysURL:"https://api.github.com/repos/twiecki/mpi4py_map/keys{/key_id}", LabelsURL:"https://api.github.com/repos/twiecki/mpi4py_map/labels{/name}", LanguagesURL:"https://api.github.com/repos/twiecki/mpi4py_map/languages", MergesURL:"https://api.github.com/repos/twiecki/mpi4py_map/merges", MilestonesURL:"https://api.github.com/repos/twiecki/mpi4py_map/milestones{/number}", NotificationsURL:"https://api.github.com/repos/twiecki/mpi4py_map/notifications{?since,all,participating}", PullsURL:"https://api.github.com/repos/twiecki/mpi4py_map/pulls{/number}", ReleasesURL:"https://api.github.com/repos/twiecki/mpi4py_map/releases{/id}", StargazersURL:"https://api.github.com/repos/twiecki/mpi4py_map/stargazers", StatusesURL:"https://api.github.com/repos/twiecki/mpi4py_map/statuses/{sha}", SubscribersURL:"https://api.github.com/repos/twiecki/mpi4py_map/subscribers", SubscriptionURL:"https://api.github.com/repos/twiecki/mpi4py_map/subscription", TagsURL:"https://api.github.com/repos/twiecki/mpi4py_map/tags", TreesURL:"https://api.github.com/repos/twiecki/mpi4py_map/git/trees{/sha}", TeamsURL:"https://api.github.com/repos/twiecki/mpi4py_map/teams"}'
