#!/bin/sh

# github-clone-all script 
gh='/path/to//github-clone-all';
creds_token="ghp_token";
# amount of repositories (max number is 1000)
amount=1000;

is_dry=$1;

start_date=$2;
end_date=$3;

if [[ -n $is_dry ]]; then
	$gh -count $amount -dest git_repos -dry -extract '(\.c|.f|.f77|.f90|.f95|.f03|.f08|.f18|.cc|.cpp|.cxx|.h)$' -token $creds_token "openmp created:$start_date..$end_date";
else
	$gh -count $amount -dest git_repos -extract '(\.c|.f|.f77|.f90|.f95|.f03|.f08|.f18|.cc|.cpp|.cxx|.h)$' -token $creds_token "openmp created:$start_date..$end_date";
fi
