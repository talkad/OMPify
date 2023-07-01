import matplotlib.pyplot as plt
import json

# aaa = {'2012': 87, '2013': 187, '2014': 301, '2015': 477, '2016': 634, '2017': 938, '2018': 1133, '2019': 1294, '2020': 1345, '2021': 1230, '2022': 1151}
# aaa = {2012: 44, 2013: 118,  2014: 191, 2015: 294, 2016: 385, 2017: 617, 2018: 732, 2019: 858,  2020: 922, 2021: 785, 2022: 475}
# plt.bar(list(aaa.keys()), list(aaa.values()))
# plt.show()


# {2021: 785, 2022: 475, 2019: 858, 2014: 191, 2016: 385, 2020: 922, 2017: 617, 2015: 294, 2018: 732, 2013: 118, 2012: 44}
# {2021: {2: 31432, 3: 30809, 4: 49056, 4.5: 1731, 5: 542}, 2022: {2: 3951, 3: 238, 4: 279, 4.5: 0, 5: 0}, 2019: {2: 35764, 3: 39687, 4: 55974, 4.5: 860, 5: 390}, 2014: {2: 2431, 3: 171, 4: 47, 4.5: 0, 5: 0}, 2016: {2: 36251, 3: 3671, 4: 4339, 4.5: 23, 5: 12}, 2020: {2: 21631, 3: 3885, 4: 4757, 4.5: 55, 5: 3}, 2017: {2: 29011, 3: 25653, 4: 32271, 4.5: 275, 5: 226}, 2015: {2: 10799, 3: 2134, 4: 2096, 4.5: 32, 5: 0}, 2018: {2: 19012, 3: 10028, 4: 13891, 4.5: 202, 5: 101}, 2013: {2: 3068, 3: 1126, 4: 1048, 4.5: 33, 5: 0}, 2012: {2: 588, 3: 0, 4: 0, 4.5: 0, 5: 0}}

# openMP
# aa ={'1-2012': 1, '2-2012': 9, '3-2012': 5, '4-2012': 9, '5-2012': 4, '6-2012': 4, '7-2012': 6, '8-2012': 6, '9-2012': 10, '10-2012': 11, '11-2012': 14, '12-2012': 6, '1-2013': 5, '2-2013': 13, '3-2013': 18, '4-2013': 17, '5-2013': 8, '6-2013': 9, '7-2013': 14, '8-2013': 12, '9-2013': 9, '10-2013': 23, '11-2013': 22, '12-2013': 26, '1-2014': 25, '2-2014': 16, '3-2014': 29, '4-2014': 29, '5-2014': 24, '6-2014': 22, '7-2014': 26, '8-2014': 16, '9-2014': 27, '10-2014': 27, '11-2014': 31, '12-2014': 27, '1-2015': 38, '2-2015': 29, '3-2015': 50, '4-2015': 38, '5-2015': 37, '6-2015': 32, '7-2015': 26, '8-2015': 34, '9-2015': 41, '10-2015': 59, '11-2015': 54, '12-2015': 45, '1-2016': 36, '2-2016': 51, '3-2016': 66, '4-2016': 56, '5-2016': 59, '6-2016': 41, '7-2016': 32, '8-2016': 28, '9-2016': 50, '10-2016': 76, '11-2016': 83, '12-2016': 53, '1-2017': 65, '2-2017': 70, '3-2017': 122, '4-2017': 89, '5-2017': 89, '6-2017': 61, '7-2017': 72, '8-2017': 60, '9-2017': 71, '10-2017': 117, '11-2017': 96, '12-2017': 98, '1-2018': 60, '2-2018': 96, '3-2018': 116, '4-2018': 110, '5-2018': 114, '6-2018': 77, '7-2018': 71, '8-2018': 69, '9-2018': 82, '10-2018': 118, '11-2018': 93, '12-2018': 91, '1-2019': 105, '2-2019': 113, '3-2019': 147, '4-2019': 135, '5-2019': 119, '6-2019': 97, '7-2019': 75, '8-2019': 73, '9-2019': 90, '10-2019': 140, '11-2019': 124, '12-2019': 121, '1-2020': 91, '2-2020': 92, '3-2020': 102, '4-2020': 152, '5-2020': 141, '6-2020': 110, '7-2020': 90, '8-2020': 78, '9-2020': 107, '10-2020': 141, '11-2020': 135, '12-2020': 125, '1-2021': 110, '2-2021': 99, '3-2021': 113, '4-2021': 116, '5-2021': 121, '6-2021': 87, '7-2021': 70, '8-2021': 97, '9-2021': 93, '10-2021': 91, '11-2021': 148, '12-2021': 120, '1-2022': 94, '2-2022': 71, '3-2022': 117, '4-2022': 134, '5-2022': 159, '6-2022': 101, '7-2022': 99, '8-2022': 48}
openmp_data = {'2012': 85, '2013': 176, '2014': 299, '2015': 483, '2016': 631, '2017': 1010, '2018': 1097, '2019': 1339, '2020': 1364, '2021': 1265, '2022': 1238}

# cilk
# aa =  {'1-2012': 3, '3-2012': 1, '4-2012': 2, '5-2012': 1, '6-2012': 1, '7-2012': 0, '8-2012': 0, '9-2012': 3, '10-2012': 1, '11-2012': 3, '12-2012': 1, '1-2013': 7, '2-2013': 1, '3-2013': 1, '4-2013': 2, '5-2013': 0, '6-2013': 2, '7-2013': 0, '8-2013': 1, '9-2013': 0, '10-2013': 0, '11-2013': 3, '12-2013': 3, '1-2014': 1, '2-2014': 2, '3-2014': 2, '4-2014': 2, '5-2014': 1, '6-2014': 1, '7-2014': 1, '8-2014': 0, '9-2014': 5, '10-2014': 2, '11-2014': 2, '12-2014': 1, '1-2015': 1, '2-2015': 2, '3-2015': 8, '4-2015': 3, '5-2015': 1, '6-2015': 2, '7-2015': 1, '8-2015': 3, '9-2015': 1, '10-2015': 0, '11-2015': 1, '12-2015': 2, '1-2016': 3, '2-2016': 3, '3-2016': 2, '4-2016': 1, '5-2016': 3, '6-2016': 0, '7-2016': 0, '8-2016': 0, '9-2016': 1, '10-2016': 2, '11-2016': 2, '12-2016': 1, '1-2017': 0, '2-2017': 1, '3-2017': 2, '4-2017': 2, '5-2017': 1, '6-2017': 1, '7-2017': 0, '8-2017': 1, '9-2017': 1, '10-2017': 4, '11-2017': 6, '12-2017': 1, '1-2018': 2, '2-2018': 5, '3-2018': 7, '4-2018': 1, '5-2018': 1, '6-2018': 1, '7-2018': 1, '8-2018': 2, '9-2018': 1, '10-2018': 1, '11-2018': 2, '12-2018': 2, '1-2019': 3, '2-2019': 1, '3-2019': 2, '4-2019': 4, '5-2019': 4, '6-2019': 1, '7-2019': 1, '8-2019': 2, '9-2019': 0, '10-2019': 9, '11-2019': 1, '12-2019': 3, '1-2020': 5, '2-2020': 4, '3-2020': 2, '4-2020': 2, '5-2020': 3, '6-2020': 2, '7-2020': 1, '8-2020': 0, '9-2020': 0, '10-2020': 1, '11-2020': 3, '12-2020': 0, '1-2021': 1, '2-2021': 1, '3-2021': 2, '4-2021': 0, '5-2021': 5, '6-2021': 0, '7-2021': 2, '8-2021': 0, '9-2021': 2, '10-2021': 1, '11-2021': 4, '12-2021': 0, '1-2022': 0, '2-2022': 1, '3-2022': 0, '4-2022': 1, '5-2022': 1, '6-2022': 0, '7-2022': 2, '8-2022': 0, '9-2022': 1, '10-2022': 1, '11-2022': 4, '12-2022': 1}
cilk_data = {'2012': 16, '2013': 20, '2014': 20, '2015': 25, '2016': 18, '2017': 20, '2018': 26, '2019': 31, '2020': 23, '2021': 18, '2022': 12}

# TBB
# aa = {'1-2012': 1, '3-2012': 1, '4-2012': 3, '5-2012': 5, '6-2012': 5, '7-2012': 2, '8-2012': 1, '9-2012': 2, '10-2012': 2, '11-2012': 1, '12-2012': 3, '1-2013': 2, '2-2013': 2, '3-2013': 3, '4-2013': 5, '5-2013': 4, '6-2013': 4, '7-2013': 6, '8-2013': 3, '9-2013': 3, '10-2013': 3, '11-2013': 2, '12-2013': 5, '1-2014': 6, '2-2014': 6, '3-2014': 6, '4-2014': 8, '5-2014': 8, '6-2014': 6, '7-2014': 6, '8-2014': 4, '9-2014': 10, '10-2014': 6, '11-2014': 7, '12-2014': 6, '1-2015': 11, '2-2015': 7, '3-2015': 5, '4-2015': 7, '5-2015': 4, '6-2015': 7, '7-2015': 5, '8-2015': 10, '9-2015': 13, '10-2015': 12, '11-2015': 12, '12-2015': 6, '1-2016': 4, '2-2016': 6, '3-2016': 7, '4-2016': 6, '5-2016': 12, '6-2016': 5, '7-2016': 8, '8-2016': 4, '9-2016': 9, '10-2016': 10, '11-2016': 13, '12-2016': 8, '1-2017': 8, '2-2017': 12, '3-2017': 9, '4-2017': 12, '5-2017': 9, '6-2017': 7, '7-2017': 16, '8-2017': 18, '9-2017': 9, '10-2017': 10, '11-2017': 15, '12-2017': 14, '1-2018': 12, '2-2018': 19, '3-2018': 6, '4-2018': 11, '5-2018': 9, '6-2018': 9, '7-2018': 7, '8-2018': 14, '9-2018': 12, '10-2018': 10, '11-2018': 15, '12-2018': 9, '1-2019': 8, '2-2019': 19, '3-2019': 16, '4-2019': 18, '5-2019': 14, '6-2019': 6, '7-2019': 11, '8-2019': 12, '9-2019': 14, '10-2019': 7, '11-2019': 20, '12-2019': 13, '1-2020': 13, '2-2020': 11, '3-2020': 6, '4-2020': 16, '5-2020': 20, '6-2020': 15, '7-2020': 9, '8-2020': 16, '9-2020': 17, '10-2020': 15, '11-2020': 19, '12-2020': 13, '1-2021': 15, '2-2021': 5, '3-2021': 17, '4-2021': 10, '5-2021': 13, '6-2021': 36, '7-2021': 18, '8-2021': 10, '9-2021': 15, '10-2021': 11, '11-2021': 20, '12-2021': 20, '1-2022': 17, '2-2022': 21, '3-2022': 14, '4-2022': 17, '5-2022': 23, '6-2022': 19, '7-2022': 30, '8-2022': 35, '9-2022': 14, '10-2022': 70, '11-2022': 20, '12-2022': 10}
tbb_data = {'2012': 26, '2013': 42, '2014': 79, '2015': 99, '2016': 92, '2017': 139, '2018': 133, '2019': 158, '2020': 170, '2021': 190, '2022': 290}

repos_per_year = {}
# aa = {'1-2012': 9, '3-2012': 9, '4-2012': 4, '5-2012': 4, '6-2012': 6, '7-2012': 5, '8-2012': 11, '9-2012': 12, '10-2012': 14, '11-2012': 8, '12-2012': 5, '1-2013': 11, '2-2013': 14, '3-2013': 18, '4-2013': 11, '5-2013': 7, '6-2013': 16, '7-2013': 9, '8-2013': 13, '9-2013': 19, '10-2013': 21, '11-2013': 23, '12-2013': 25, '1-2014': 20, '2-2014': 28, '3-2014': 30, '4-2014': 22, '5-2014': 22, '6-2014': 22, '7-2014': 16, '8-2014': 24, '9-2014': 20, '10-2014': 35, '11-2014': 30, '12-2014': 32, '1-2015': 34, '2-2015': 36, '3-2015': 41, '4-2015': 38, '5-2015': 44, '6-2015': 21, '7-2015': 28, '8-2015': 40, '9-2015': 45, '10-2015': 60, '11-2015': 54, '12-2015': 36, '1-2016': 38, '2-2016': 56, '3-2016': 64, '4-2016': 49, '5-2016': 56, '6-2016': 41, '7-2016': 30, '8-2016': 28, '9-2016': 54, '10-2016': 81, '11-2016': 79, '12-2016': 58, '1-2017': 59, '2-2017': 78, '3-2017': 122, '4-2017': 92, '5-2017': 80, '6-2017': 63, '7-2017': 71, '8-2017': 56, '9-2017': 70, '10-2017': 96, '11-2017': 95, '12-2017': 56, '1-2018': 104, '2-2018': 114, '3-2018': 109, '4-2018': 109, '5-2018': 84, '6-2018': 65, '7-2018': 71, '8-2018': 78, '9-2018': 105, '10-2018': 103, '11-2018': 97, '12-2018': 94, '1-2019': 106, '2-2019': 142, '3-2019': 130, '4-2019': 118, '5-2019': 104, '6-2019': 81, '7-2019': 65, '8-2019': 84, '9-2019': 114, '10-2019': 136, '11-2019': 116, '12-2019': 98, '1-2020': 86, '2-2020': 110, '3-2020': 110, '4-2020': 151, '5-2020': 136, '6-2020': 88, '7-2020': 86, '8-2020': 85, '9-2020': 114, '10-2020': 145, '11-2020': 127, '12-2020': 107, '1-2021': 97, '2-2021': 108, '3-2021': 111, '4-2021': 112, '5-2021': 119, '6-2021': 72, '7-2021': 83, '8-2021': 92, '9-2021': 88, '10-2021': 113, '11-2021': 136, '12-2021': 99, '1-2022': 95, '2-2022': 92, '3-2022': 113, '4-2022': 136, '5-2022': 154, '6-2022': 94, '7-2022': 83, '8-2022': 89, '9-2022': 117, '10-2022': 98, '11-2022': 80}
# for k,v in aa.items():
#     year = k[k.find('-')+1:]
#     if year not in repos_per_year:
#         repos_per_year[year] = 0

#     repos_per_year[year] += v

# print(repos_per_year)

# with open("repo2lines.json", "r") as f:
#     repo2lines = json.load(f)

#  with open("sample.json", "w") as f:
#     json.dump(self.id2path, f, sort_keys=True, indent=4, separators=(',', ': '))


# Convert the dictionaries to lists
years = list(openmp_data.keys())
openmp_usage = list(openmp_data.values())
cilk_usage = list(cilk_data.values())
tbb_usage = list(tbb_data.values())

# Create the plot
fig, ax = plt.subplots(figsize=(10, 6))
ax.plot(years, openmp_usage, label='OpenMP')
ax.plot(years, cilk_usage, label='Cilk')
ax.plot(years, tbb_usage, label='TBB')
ax.set_xlabel('Year', fontsize=18)
ax.set_ylabel('# Repositories', fontsize=18)
# ax.set_title('Usage of OpenMP, Cilk, and TBB libraries')
ax.legend()
plt.show()
# plt.savefig('repos_per_year.eps', format='eps')
