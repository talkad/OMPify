import os


# maybe create this class in a py module
repos_dir = '/home/talkad/Downloads/thesis/data_gathering_script/git_repos'


def get_headers(repo_name):
    '''
    For a given repo, return all headers file relative path 
    '''
    headers = []
    path_length = len(repos_dir) + len (repo_name) + 2              # remove two '/'
    omp_repo = os.path.join(repos_dir, repo_name)

    for idx, (root, dirs, files) in enumerate(os.walk(omp_repo)):
        for file_name in files:
            ext = os.path.splitext(file_name)[1].lower()
            
            if ext in '.h':
                headers.append(os.path.join(root, file_name)[path_length: ])

    return headers


for header in get_headers('0xDCA'):
    print(header)