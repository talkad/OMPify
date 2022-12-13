import matplotlib.pyplot as plt
from collections import Counter
from datetime import date
import pickle


def visualize_keywords(metadata_path):
    '''
    plot pi graph of all keywords

    {'mpi': 487, 'openmp': 1585, 'cuda': 341, 'cpp': 272, 'parallel-computing': 322, 'c': 325, 'parallel-programming': 237, 'hpc': 115, 'openmp-parallelization': 111, 'openmpi': 105, 'parallel': 142}
    {'c-plus-plus': 81, 'mpi': 487, 'openmp': 1585, 'cuda': 341, 'opencl': 81, 'cpp': 272, 'gpu': 74, 'parallel-computing': 322, 'fortran': 57, 'c': 325, 'parallel-programming': 237, 'hpc': 115, 'pthreads': 88, 'openmp-parallelization': 111, 'openmpi': 105, 'multithreading': 95, 'parallel': 142, 'python': 59, 'simd': 51, 'high-performance-computing': 65}
    '''
    keywords = []
    top_keywords = {}

    with open(metadata_path, 'rb') as f:
        data = pickle.load(f)

        for metadata in data:
            keywords += metadata['keywords']

    # return most used keywords
    for k, v in Counter(keywords).items():
        if v > 50 and len(k) > 0:
            top_keywords[k] = v

    return top_keywords


def lastUpdate_info(metadata_path):
    '''
    get last update rate
    (8908, 1436)
    '''
    total, last_update = 0, 0
    UPDATE_THRESHOLD = 6 * 30
    today_date = date.today()

    with open(metadata_path, 'rb') as f:
        data = pickle.load(f)

        for metadata in data:
            total += 1

            if (today_date - metadata['update_date']).days < UPDATE_THRESHOLD:
                last_update += 1

    return total, last_update


def count_lang(metadata_path):
    '''
    get the amount of use of each programming language

    {'C': 3556, 'Python': 164, 'C++': 3442, 'Shell': 157, 'FORTRAN': 40, 'Java': 74, 'JavaScript': 21, 'Cuda': 135, 'Fortran': 204, 'TeX': 69, 'Jupyter Notebook': 132, 'C#': 13, 'Makefile': 66, 'CMake': 40, 'R': 14, 'HTML': 39, 'Assembly': 13, 'Dockerfile': 50}
    '''
    langs = []
    langs_count = {}

    with open(metadata_path, 'rb') as f:
        data = pickle.load(f)

        for metadata in data:
            langs.append(metadata['lang'])

    # return most used keywords
    for k, v in Counter(langs).items():
        if v > 10 and len(k) > 0:
            langs_count[k] = v

    return langs_count


def count_users_info(metadata_path):
    '''
    Get company, location and email countings

    amount of national labs 33
    {'lanl.gov': 10, 'llnl.gov': 6, 'pnnl.gov': 5, 'olcf.ornl.gov': 3, 'bnl.gov': 3, 'sandia.gov': 2, 'mcs.anl.gov': 1, 'usgs.gov': 1, 'fnal.gov': 1, 'ornl.gov': 1}
    ({'Google': 17, 'Carnegie Mellon University': 15, 'Intel': 15, 'NVIDIA': 16, 'Microsoft': 39, 'Amazon': 11}, 
    {'Spain': 20, 'London': 32, 'Germany': 43, 'Boston, MA': 12, 'New York': 27, 'Chicago, IL': 11, 'China': 13, 'Singapore': 26, 'Berlin, Germany': 19, 'Paris, France': 18, 'Tokyo': 13, 'Poland': 29, 'Stockholm': 18, 'Seattle, WA': 20, 'UK': 15, 'Taiwan': 16, 'Beijing, China': 11, 'Santa Clara, CA': 12, 'Pittsburgh, PA': 13, 'Portugal': 12, 'France': 48, 'Berlin': 17, 'San Francisco': 19, 'Japan': 13, 'USA': 24, 'San Francisco, CA': 18, 'London, UK': 18, 'Italy': 31, 'Berkeley, CA': 12, 'Edinburgh, UK': 13, 'Austin, TX': 17, 'Moscow': 32, 'United States': 45, 'Dublin, Ireland': 23, 'Egypt': 15, 'Brazil': 60, 'Bangalore': 29, 'Bangalore, India': 15, 'Hsinchu, Taiwan': 13, 'Moscow, Russia': 31, 'Canada': 17, 'Greece': 29, 'Athens, Greece': 36, 'Barcelona': 23, 'Paris': 27, 'Melbourne': 13, 'Los Angeles': 12, 'India': 61, 'Brasil': 13, 'Bristol, UK': 14, 'Munich, Germany': 12, 'Thessaloniki, Greece': 17, 'New York, NY': 19, 'Israel': 14, 'Hyderabad': 18, 'Hong Kong': 15, 'United Kingdom': 14, 'Hyderabad, India': 15, 'Russia': 29, 'Beijing': 16, 'Dallas, TX': 12, 'United States of America': 18, 'Madrid, Spain': 11, 'Tokyo, Japan': 13, 'Istanbul': 11, 'Redmond, WA': 11, 'Karachi, Pakistan': 11}, 
    {'gmail': 1763, 'protonmail': 17, 'hotmail': 74, 'cs': 12, 'qq': 30, 'intel': 24, 'live': 15, 'outlook': 65, '163': 19, 'mail': 29, 'yahoo': 31, 'googlegroups': 18, 'yandex': 15, 'epcc': 11})
    '''
    national_labs = []
    keywords = []
    companies, locations, emails = [], [], []
    top_companies, top_locations, top_emails = {}, {}, {}

    with open(metadata_path, 'rb') as f:
        data = pickle.load(f)

        for metadata in data:
            companies.append(metadata['company'])
            locations.append(metadata['location'])
            email = '' if metadata['email'] is None else metadata['email']            
            email = email[email.find('@')+1:]
            emails.append(email[:email.find('.')])

            if metadata['email'] is not None and '.gov' in metadata['email']:
                national_labs.append(email)

    for top, count in zip([top_companies, top_locations, top_emails], [Counter(companies), Counter(locations), Counter(emails)]):
        for k, v in count.items():
            if k is not None and v > 10 and len(k) > 0:
                top[k] = v

    print(f'amount of national labs {len(national_labs)}')
    print(national_labs)
    return top_companies, top_locations, top_emails


# print(count_lang('/home/talkad/Downloads/thesis/data_gathering_script/visualization/metadata_mpi.pickle'))
# print(lastUpdate_info('/home/talkad/Downloads/thesis/data_gathering_script/visualization/metadata_mpi.pickle'))

# print(count_users_info('/home/talkad/Downloads/thesis/data_gathering_script/visualization/metadata_user_mpi.pickle'))

# {'Python': 1522, 'C++': 4534, 'C': 5690, 'Matlab': 22, 'JavaScript': 218, 'Haskell': 15, 'Cuda': 125, 'FORTRAN': 79, 'PHP': 80, 'Ruby': 34, 'Perl': 35, 'Julia': 95, 'Assembly': 44, 'Go': 52, 'R': 98, 'Fortran': 379, 'Objective-C': 23, 'C#': 127, 'HTML': 267, 'Scala': 12, '09, NodeID:': 14, 'CMake': 39, 'TeX': 129, 'M4': 11, 'Dart': 13, 'Makefile': 78, 'CSS': 73, 'Jupyter Notebook': 307, 'MATLAB': 35, 'TypeScript': 33, 'Kotlin': 37, 'Vue': 11}
# # (17349, 2584)
# amount of national labs 98
# ['lanl.gov', 'lanl.gov', 'llnl.gov', 'llnl.gov', 'llnl.gov', 'lanl.gov', 'llnl.gov', 'lanl.gov', 'llnl.gov', 'lanl.gov', 'lanl.gov', 'lanl.gov', 'lanl.gov', 'lbl.gov', 'lanl.gov', 'olcf.ornl.gov', 'lanl.gov', 'llnl.gov', 'lanl.gov', 'olcf.ornl.gov', 'lbl.gov', 'llnl.gov', 'nasa.gov', 'mcs.anl.gov', 'nist.gov', 'usgs.gov', 'llnl.gov', 'elist.ornl.gov', 'lanl.gov', 'elist.ornl.gov', 'llnl.gov', 'lanl.gov', 'llnl.gov', 'llnl.gov', 'llnl.gov', 'llnl.gov', 'mcs.anl.gov', 'llnl.gov', 'lanl.gov', 'noaa.gov', 'llnl.gov', 'ehealth.gov.ua', 'ehealth.gov.ua', 'ehealth.gov.ua', 'lbl.gov', 'sandia.gov', 'llnl.gov', 'llnl.gov', 'llnl.gov', 'cdc.gov', 'sandia.gov', 'llnl.gov', 'llnl.gov', 'llnl.gov', 'llnl.gov', 'llnl.gov', 'usgs.gov', 'lists.mcs.anl.gov', 'lists.mcs.anl.gov', 'lists.mcs.anl.gov', 'lists.mcs.anl.gov', 'lanl.gov', 'lanl.gov', 'lists.mcs.anl.gov', 'lanl.gov', 'llnl.gov', 'llnl.gov', 'usgs.gov', 'ehealth.gov.ua', 'lanl.gov', 'ncats.nih.gov', 'llnl.gov', 'ornl.gov', 'lbl.gov', 'ornl.gov', 'fnal.gov', 'pnnl.gov', 'llnl.gov', 'elist.ornl.gov', 'nist.gov', 'llnl.gov', 'llnl.gov', 'llnl.gov', 'pnnl.gov', 'pnnl.gov', 'sandia.gov', 'lists.arc.nasa.gov', 'pnnl.gov', 'contractor.usgs.gov', 'lanl.gov', 'pnnl.gov', 'llnl.gov', 'jpl.nasa.gov', 'lanl.gov', 'nist.gov', 'lanl.gov', 'lanl.gov', 'pnnl.gov']
# ({'Google': 35, 'NVIDIA': 27, 'Intel': 16, '@google': 11, 'Microsoft': 56, '@nvidia': 22, 'Stanford University': 11, '@NVIDIA': 11, 'Tsinghua University': 18, 'Argonne National Laboratory': 15, 'Carnegie Mellon University': 12, 'IBM': 14, 'Georgia Institute of Technology': 15, 'University of Michigan': 11, 'Amazon': 17, 'Imperial College London': 17, 'Perimeter Institute for Theoretical Physics': 12, 'Laboratory for Biosignal Processing': 14, 'IIIT Hyderabad': 11, 'None': 12, 'Cloud Data Engineer at UST': 17}, {'Europe': 12, 'UK': 23, 'Russia': 31, 'Spain': 45, 'Chicago': 19, 'Bucharest, Romania': 34, 'Switzerland': 22, 'Oslo, Norway': 17, 'Boston, MA': 23, 'United Kingdom': 34, 'Seattle, WA': 47, 'Germany': 99, 'Amsterdam': 17, 'USA': 38, 'Argentina': 15, 'Livermore, CA, USA': 27, 'London, UK': 47, 'Sweden': 15, 'Boulder, CO': 21, 'Saint-Petersburg': 18, 'Singapore': 40, 'Hangzhou, China': 12, 'Tokyo, Japan': 32, 'Poland': 44, 'Melbourne, Australia': 30, 'San Jose, CA': 17, 'United States': 88, 'Atlanta, GA': 17, 'Paris': 28, 'Zurich': 18, 'Chicago, IL': 33, 'London': 66, 'Berkeley': 12, 'San Francisco': 19, 'Seattle': 15, 'Hamburg, Germany': 11, 'Nizhny Novgorod, Russia': 13, 'Beijing': 50, 'Moscow': 84, 'Shenzhen, China': 11, 'Berlin': 32, 'Stockholm': 27, 'Hong Kong': 13, 'Mountain View, CA': 14, 'Hyderabad': 24, 'Munich, Germany': 38, 'India': 60, 'New York': 39, 'Denver, CO': 11, 'Portland, OR': 16, 'Paris, France': 37, 'Hamburg': 12, 'New Delhi': 11, 'Beijing, China': 38, 'United States of America': 29, 'Shanghai': 16, 'Montreal': 15, 'Canada': 32, 'Leipzig, Germany': 11, 'Tokyo': 23, 'Helsinki, Finland': 33, 'Berkeley, CA': 24, 'Pittsburgh': 12, 'Japan': 23, 'London, United Kingdom': 12, 'Melbourne': 23, 'Berlin, Germany': 22, 'San Francisco, CA': 18, 'Kyiv, Ukraine': 21, 'Russia, Moscow': 13, 'France': 71, 'Moscow, Russia': 45, 'Dublin, Ireland': 14, 'Sri Lanka': 13, 'Bangalore': 27, 'Toronto': 20, 'Bucharest': 26, 'Stockholm, Sweden': 20, 'Austin, TX': 19, 'Athens, Greece': 44, 'Colombia': 17, 'China': 23, 'Cambridge, UK': 17, 'Bangalore, India': 16, 'Barcelona': 29, 'Atlanta': 17, 'New York, NY': 18, 'Minneapolis': 14, 'Italy': 51, 'Pakistan': 13, 'Pittsburgh, PA': 11, 'Edinburgh, UK': 27, 'Hsinchu, Taiwan': 12, 'Greece': 25, 'Barcelona, Spain': 14, 'Australia': 14, 'Dallas, TX': 11, 'Cairo, Egypt': 14, 'Cambridge, MA': 15, 'Costa Rica': 12, 'Egypt': 12, 'Los Angeles, CA': 17, 'Kyiv': 15, 'Ukraine': 14, 'Warsaw': 20, 'Shanghai, China': 14, 'Brasil': 11, 'Athens': 12, 'Netherlands': 14, 'Munich': 21, 'Indonesia': 23, 'Taiwan': 20, 'Boston': 14, 'Earth': 14, 'Brazil': 46, 'Bristol, UK': 13, 'Lausanne, Switzerland': 11, 'Copenhagen, Denmark': 14, 'Portugal': 15, 'Istanbul': 16, 'Thessaloniki, Greece': 17, 'Romania': 19, 'Russia, Novosibirsk': 11, 'Israel': 20, 'Warsaw, Poland': 15, 'São Paulo': 13, 'Los Angeles': 17, 'Waterloo, Ontario, Canada': 12, 'Leipzig': 17, 'Turkey': 11, 'Novosibirsk': 11, 'Latvia': 12, 'Granada (Spain)': 17, 'Islamabad, Pakistan': 16}, {'gmail': 2962, 'lanl': 22, 'gatech': 11, 'llnl': 31, 'protonmail': 34, 'outlook': 91, 'hotmail': 115, 'yandex': 54, 'cs': 18, 'acm': 25, 'googlegroups': 32, 'yahoo': 58, 'intel': 30, 'microsoft': 16, 'qq': 62, '163': 33, 'illinois': 14, 'live': 22, 'epcc': 18, 'lists': 20, 'icloud': 14, 'g': 14, 'mail': 25})