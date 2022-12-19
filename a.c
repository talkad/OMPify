
//
// Created by hezhiwei on 6/26/20.
//
#include "omp_group_by.h"

void omp_first_char_count_string_sort(char*** str_arr_p, size_t num, size_t** begin_pos_p, size_t** end_pos_p)
{
    char** str_arr = *str_arr_p;
    size_t* cnt = calloc(SIZE_OF_CNT, sizeof(size_t)); //use for counting sort
    size_t* beg_pos = calloc(SIZE_OF_CNT, sizeof(size_t));
    size_t* thread_sum;
    size_t* cnt_private;
    size_t it = 0, pre_sum = 0;
    int i=0;
    uint8_t * first_lets = calloc(num, sizeof(char)); //store the first elem of each string
    char** temp = calloc(num, sizeof(char*)); //store the sorted array
    if (!(beg_pos && cnt && first_lets && temp)) {
        fprintf(stderr, "fail to malloc in omp_first_char_count_string_sort");
        exit(-1);
    }


#pragma omp parallel default(none) private(i, it) shared(num, first_lets, str_arr, cnt, pre_sum, temp, cnt_private, thread_sum, stderr, beg_pos)
    {
        const int num_threads = omp_get_num_threads();
        const int my_id = omp_get_thread_num();

        // construct a array of size [num_threads X SIZE_OF_CNT]
        // cnt_private[i, j] storage # of appearance of char j in thread i.
#pragma omp single
        {
            cnt_private = calloc(num_threads * (SIZE_OF_CNT), sizeof(size_t));
            if (!cnt_private) {
                fprintf(stderr, "fail to malloc in omp_first_char_count_string_sort");
                exit(-1);
            }
        }

        //save the first letter of each string
#pragma omp for schedule(static)
        for (it = 0; it < num; ++it) {
            first_lets[it] = uint16_map_to_idx((str_arr[it][0]<<8)|str_arr[it][1]);
        }

        //cal # of appearance of each letter in each thread
#pragma omp for schedule(static)
        for (it = 0; it < num; ++it) {
            ++cnt_private[my_id*(SIZE_OF_CNT) + first_lets[it]];
        }

        // add the result in each thread to cnt
#pragma omp for schedule(static)
        for (i = 0; i < SIZE_OF_CNT; ++i) {
            int k;
            for(k=0; k < num_threads; ++k) {
                cnt[i] += cnt_private[k*(SIZE_OF_CNT) + i];
            }
        }


        // cal begin pos of each category(exclusive prefix-sum)
#pragma omp single
        {
            thread_sum = calloc((num_threads + 1), sizeof(size_t));
            if (!thread_sum) {
                fprintf(stderr, "fail to malloc in omp_first_char_count_string_sort");
                exit(-1);
            }
        }

        // save the appearance times of the last elements of each thread
        size_t last_le_times;
#pragma omp for schedule(static)
        for (i = 0; i < SIZE_OF_CNT; ++i) {
            last_le_times = cnt[i];
        }

        //cal prefix sum in each thread
        size_t curr_sum, sum = 0; //private
#pragma omp for schedule(static)
        for (i = 0; i < SIZE_OF_CNT; ++i) {
            curr_sum = sum;
            sum = cnt[i];
            cnt[i] = curr_sum;
            sum += curr_sum;
        }


        //store (max prefix sum + the appearance times of the last elements) of each thread
        thread_sum[my_id + 1] = curr_sum + last_le_times;

#pragma omp barrier

        //store offset of each thread
        size_t offset = 0; //private
        for (i = 0; i < (my_id + 1); ++i) {
            offset += thread_sum[i];
        }

        // cal the begin pos of each group in cnt
#pragma omp for schedule(static)
        for (i = 0; i < SIZE_OF_CNT; ++i) {
            cnt[i] += offset;
        }

        // save begin pos of each group into beg_pos and sort
#pragma omp single
        {
            memcpy(beg_pos, cnt, sizeof(size_t)*(SIZE_OF_CNT));
            for (it = 0; it < num; ++it) {
                temp[cnt[first_lets[it]]++] = str_arr[it];
            }
        };
    };


    //return value
    *str_arr_p = temp;
    *begin_pos_p = beg_pos;
    *end_pos_p = cnt;

    //free
    free(first_lets);
    free(cnt_private);
    free(thread_sum);
}

void omp_first_char_count_string_sort2(char*** str_arr_p, size_t num, size_t** begin_pos_p, size_t** end_pos_p)
{
    char** str_arr = *str_arr_p;
    size_t* cnt = calloc(SIZE_OF_CNT, sizeof(size_t)); //use for counting sort
    size_t* beg_pos = calloc(SIZE_OF_CNT, sizeof(size_t));
    size_t* thread_sum;
    size_t* cnt_private;
    size_t it = 0, pre_sum = 0;
    int i = 0, k = 0;
    uint8_t * first_lets = calloc(num, sizeof(char)); //store the first elem of each string
    char** temp = calloc(num, sizeof(char*)); //store the sorted array
    if (!(beg_pos && cnt && first_lets && temp)) {
        fprintf(stderr, "fail to malloc in omp_first_char_count_string_sort");
        exit(-1);
    }


#pragma omp parallel default(none) private(i, it, k) shared(num, first_lets, str_arr, cnt, pre_sum, temp, cnt_private, thread_sum, stderr, beg_pos)
    {
        const int num_threads = omp_get_num_threads();
        const int my_id = omp_get_thread_num();

        // construct a array of size [num_threads X SIZE_OF_CNT]
        // cnt_private[i, j] storage # of appearance of char j in thread i.
#pragma omp single
        {
            cnt_private = calloc(num_threads * (SIZE_OF_CNT), sizeof(size_t));
            if (!cnt_private) {
                fprintf(stderr, "fail to malloc in omp_first_char_count_string_sort");
                exit(-1);
            }
        }

        //save the first letter of each string
#pragma omp for schedule(static)
        for (it = 0; it < num; ++it) {
//            printf("%s\n",str_arr[it]);
            first_lets[it] = uint16_map_to_idx((str_arr[it][0]<<8)|str_arr[it][1]);
        }

        //cal # of appearance of each letter in each thread
#pragma omp for schedule(static)
        for (it = 0; it < num; ++it) {
            ++cnt_private[my_id * (SIZE_OF_CNT) + first_lets[it]];
        }

        // add the result in each thread to cnt
#pragma omp for schedule(static)
        for (i = 0; i < SIZE_OF_CNT; ++i) {
            for (k = 0; k < num_threads; ++k) {
                cnt[i] += cnt_private[k * (SIZE_OF_CNT) + i];
            }
        }


        // cal begin pos of each category(exclusive prefix-sum)
#pragma omp single
        {
            thread_sum = calloc((num_threads + 1), sizeof(size_t));
            if (!thread_sum) {
                fprintf(stderr, "fail to malloc in omp_first_char_count_string_sort");
                exit(-1);
            }
        }

        // save the appearance times of the last elements of each thread
        size_t last_le_times;
#pragma omp for schedule(static)
        for (i = 0; i < SIZE_OF_CNT; ++i) {
            last_le_times = cnt[i];
        }

        //cal prefix sum in each thread
        size_t curr_sum, sum = 0; //private
#pragma omp for schedule(static)
        for (i = 0; i < SIZE_OF_CNT; ++i) {
            curr_sum = sum;
            sum = cnt[i];
            cnt[i] = curr_sum;
            sum += curr_sum;
        }

        //store (max prefix sum + the appearance times of the last elements) of each thread
        thread_sum[my_id + 1] = curr_sum + last_le_times;

#pragma omp barrier

        //store offset of each thread
        size_t offset = 0; //private
        for (i = 0; i < (my_id + 1); ++i) {
            offset += thread_sum[i];
        }

        // cal the begin pos of each group in cnt
#pragma omp for schedule(static)
        for (i = 0; i < SIZE_OF_CNT; ++i) {
            cnt[i] += offset;
        }

        // cal the begin pos of each group for each process in cnt private
#pragma omp for schedule(static)
        for (i = 0; i < SIZE_OF_CNT; ++i) {
            curr_sum = 0;
            sum = 0;
            for (k = 0; k < num_threads; ++k) {
                curr_sum = sum;
                sum = cnt_private[k * (SIZE_OF_CNT) + i];
                cnt_private[k * (SIZE_OF_CNT) + i] = curr_sum;
                sum += curr_sum;
            }
        }

#pragma omp for schedule(static)
        for (i = 0; i < SIZE_OF_CNT; ++i) {
            for (k = 0; k < num_threads; ++k) {
                cnt_private[k * (SIZE_OF_CNT) + i] += cnt[i];
            }
        }

        // save begin pos of each group into beg_pos
//#pragma omp single
//        {
//            memcpy(beg_pos, cnt, sizeof(size_t) * (SIZE_OF_CNT));
//        }
#pragma omp for schedule(static)
        for(it = 0; it < SIZE_OF_CNT; it++){
            beg_pos[it] = cnt[it];
        }

        //sort
#pragma omp for schedule(static)
        for (it = 0; it < num; ++it) {
            temp[cnt_private[my_id * (SIZE_OF_CNT) + first_lets[it]]++] = str_arr[it];
        }

//#pragma omp single
//        {
//            memcpy(cnt, cnt_private + (num_threads - 1)*(SIZE_OF_CNT), sizeof(size_t) * (SIZE_OF_CNT));
//        }

#pragma omp for schedule(static)
        for(it = 0; it < SIZE_OF_CNT; it++){
            cnt[it] = cnt_private[(num_threads - 1)*(SIZE_OF_CNT) + it];
        }


    }

    //return value
    *str_arr_p = temp;
    *begin_pos_p = beg_pos;
    *end_pos_p = cnt;

    //free
    free(first_lets);
    free(cnt_private);
    free(thread_sum);
}

void omp_quick_sort_partial(char** arr, size_t *starts, size_t *ends, int level){
    size_t i;
#pragma omp parallel for default(none) private(i) shared(arr, starts, ends, level) schedule(dynamic)
    for(i=0; i < SIZE_OF_CNT; ++i){
        const int myid = omp_get_thread_num();
        if(ends[i] != starts[i]){
            quick_sort(arr+starts[i], ends[i] - starts[i], level);
        }
    }
}

void omp_radix_sort_partial(char** arr, size_t *starts, size_t *ends, int level){
    size_t i;
#pragma omp parallel for default(none) private(i) shared(arr, starts, ends, level) schedule(dynamic)
    for(i=0; i < SIZE_OF_CNT; ++i){
        if(ends[i] != starts[i]){
            radix_sort_main(arr+starts[i], ends[i] - starts[i], level);
        }
    }
}

void omp_assign_group(char** arr, char** group_keys, size_t* group_lens, size_t* num_groups_p, size_t num_strings, size_t* prefix_starts, size_t* prefix_ends) {
    size_t* group_assigned = calloc(num_strings, sizeof(size_t));
    size_t* shared_num_groups;
    size_t* max_prefix_sums;
    size_t it, it2;
    int i, num_groups=0;

    if(!group_assigned){
        fprintf(stderr,"cant allocate memory in omp_assign_group");
    }

#pragma omp parallel default(none) private(i, it, it2) shared(stdout, group_lens, group_keys,shared_num_groups, num_groups, group_assigned, num_strings, arr, max_prefix_sums, stderr, prefix_ends, prefix_starts)
    {
        const int num_threads = omp_get_num_threads();
        const int my_id = omp_get_thread_num();

#pragma omp single
        {
            shared_num_groups = calloc(num_threads + 1, sizeof(size_t));
            if (!shared_num_groups) {
                fprintf(stderr, "fail to malloc in omp_assign_group");
                exit(-1);
            }
        }

#pragma omp for schedule(static)
        for (i = 1; i < num_strings; ++i) {
            if (strcmp(arr[i], arr[i - 1]) != 0) {
                group_assigned[i] = 1;
            }
        }

#pragma omp single
        {
            max_prefix_sums = calloc(num_threads + 1, sizeof(size_t));
            if (!max_prefix_sums) {
                fprintf(stderr, "fail to malloc in omp_assign_group");
                exit(-1);
            }
        }

        size_t sum = 0; //private
#pragma omp for schedule(static)
        for (i = 0; i < num_strings; ++i) {
            sum += group_assigned[i];
            group_assigned[i] = sum;
        }


        max_prefix_sums[my_id + 1] = sum;
#pragma omp barrier
        //todo:if it's necessary to parallel?
#pragma omp single
        {
            for(i = 1;i<num_threads;++i){
                max_prefix_sums[i] += max_prefix_sums[i-1];
            }
        };

#pragma omp for schedule(static)
        for (i = 0; i < num_strings; ++i) {
            group_assigned[i] += max_prefix_sums[my_id];
        }


        size_t private_num_groups = 0; //private

        char** private_keys_arr = calloc(num_strings, sizeof(char *)); //private
        int* private_sizes_arr = calloc(num_strings, sizeof(int ));
        if(!(private_keys_arr && private_sizes_arr)){
            fprintf(stderr,"cant allocate memory in omp_assign_group");
        }

#pragma omp barrier

#pragma omp for schedule(dynamic)
        for (it = 0; it < SIZE_OF_CNT; ++it) {
            size_t end_pos = prefix_ends[it];
            size_t beg_pos = prefix_starts[it];
            if(beg_pos == end_pos){
                continue;
            }
            else {
                size_t last_group_id=(size_t)-1;
                for(it2 = beg_pos; it2< end_pos; ++it2){
                    if(group_assigned[it2] != last_group_id){
                        //new group
                        last_group_id = group_assigned[it2];
                        private_keys_arr[private_num_groups] = arr[it2]; //save group key
                        private_sizes_arr[private_num_groups] = 1; //save group size
                        ++private_num_groups;
                    }else{
                        //old group
                        private_sizes_arr[private_num_groups-1] += 1; //add group size
                    }
                }
            }

        }

        shared_num_groups[my_id+1] = private_num_groups;
#pragma omp barrier

        //cal num_groups
#pragma omp for reduction(+:num_groups)
        for(i = 1; i < num_threads + 1; ++i){
            num_groups += shared_num_groups[i];
        }

        size_t offset = 0; //private
        for (i = 0; i < my_id + 1; ++i) {
            offset += shared_num_groups[i];
        }

        for(it = 0; it < private_num_groups; ++it){
            group_keys[offset + it] = private_keys_arr[it];
            group_lens[offset + it] = private_sizes_arr[it];
        }

        free(private_sizes_arr);
        free(private_keys_arr);

    }

    *num_groups_p = num_groups;
    free(shared_num_groups);
    free(max_prefix_sums);
    free(group_assigned);
} // end of omp_assign_group

void omp_group_by(char **str_arr, size_t num_rows, char **group_keys, size_t *group_lens, size_t* num_groups_p) {
    size_t* begin_pos, *end_pos;
    size_t n = 0;
    size_t i;
    double start, end;

    omp_first_char_count_string_sort2(&str_arr, num_rows, &begin_pos, &end_pos);
    omp_radix_sort_partial(str_arr, begin_pos, end_pos, 1);
    omp_assign_group(str_arr, group_keys, group_lens, num_groups_p, num_rows, begin_pos, end_pos);

    free(begin_pos);
    free(end_pos);
    free(str_arr);