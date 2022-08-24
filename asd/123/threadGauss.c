
   extern struct fmt_main fmt_padlock;
//   john_register_one(&fmt_padlock);

  static struct custom_salt {
        int version;
        int iterations;
        int tag_len;
        int saltlen;
        int ctlen;
        int addlen;
        unsigned char salt[64];
        unsigned char iv[16];
        unsigned char tag[128];
        unsigned char add[128];
        unsigned char ct[4096];
  } *cur_salt;
  
  static char (*saved_key)[PLAINTEXT_LENGTH + 1];
  static int *saved_len;
  static int *cracked, cracked_count;
  
  static void init(struct fmt_main *self)
  {
        omp_autotune(self, OMP_SCALE);
  
        saved_key = mem_calloc(sizeof(*saved_key), self->params.max_keys_per_crypt);
        saved_len = mem_calloc(self->params.max_keys_per_crypt, sizeof(*saved_len));
        cracked = mem_calloc(sizeof(*cracked), self->params.max_keys_per_crypt);
        cracked_count = self->params.max_keys_per_crypt;
  }
  
  static void done(void)
  {
        MEM_FREE(cracked);
        MEM_FREE(saved_key);
        MEM_FREE(saved_len);
  }
  
  static int valid(char *ciphertext, struct fmt_main *self)
  {
        char *p = ciphertext, *ctcopy, *keeptr;
        int extra;
        int res;
  
        if (strncmp(ciphertext, FORMAT_TAG, FORMAT_TAG_LENGTH))
                return 0;
        ctcopy = xstrdup(ciphertext);
        keeptr = ctcopy;
        ctcopy += FORMAT_TAG_LENGTH;
        if ((p = strtokm(ctcopy, "$")) == NULL)                 goto bail;
        if (!isdec(p))
                goto bail;
        if ((p = strtokm(NULL, "$")) == NULL)           goto bail;
        if (!isdec(p))
                goto bail;
        if ((p = strtokm(NULL, "$")) == NULL)           goto bail;
        if (!isdec(p))
                goto bail;
        res = atoi(p);
        if (res != 64 && res != 128 && res != 96)
                goto bail;
        if ((p = strtokm(NULL, "$")) == NULL)           goto bail;
        if (!isdec(p))
                goto bail;
        res = atoi(p);
        if (res > 128)
                goto bail;
        if ((p = strtokm(NULL, "$")) == NULL)           goto bail;
        if (hexlenl(p, &extra) != res * 2 || extra)
                goto bail;
        if (!ishexlc(p))
                goto bail;
        if ((p = strtokm(NULL, "$")) == NULL)           goto bail;
        if (hexlenl(p, &extra) != 16 * 2 || extra)
                goto bail;
        if (!ishexlc(p))
                goto bail;
        if ((p = strtokm(NULL, "$")) == NULL)           goto bail;
        if (!isdec(p))
                goto bail;
        res = atoi(p);
        if (res > 128)
                goto bail;
        if ((p = strtokm(NULL, "$")) == NULL)           goto bail;
        if (hexlenl(p, &extra) != res * 2 || extra)
                goto bail;
        if (!ishexlc(p))
                goto bail;
        if ((p = strtokm(NULL, "$")) == NULL)           goto bail;
        if (!isdec(p))
                goto bail;
        res = atoi(p);
        if (res > 4096)
                goto bail;
        if ((p = strtokm(NULL, "$")) == NULL)           goto bail;
        if (hexlenl(p, &extra) != res * 2 || extra)
                goto bail;
        if (!ishexlc(p))
                goto bail;
  
        MEM_FREE(keeptr);
        return 1;
  
  bail:
        MEM_FREE(keeptr);
        return 0;
  }
  
  static void *get_salt(char *ciphertext)
  {
        static struct custom_salt cs;
        int i;
        char *p = ciphertext, *ctcopy, *keeptr;
        memset(&cs, 0, sizeof(cs));
  
        ctcopy = xstrdup(ciphertext);
        keeptr = ctcopy;
        ctcopy += FORMAT_TAG_LENGTH;
        p = strtokm(ctcopy, "$");
        cs.version = atoi(p);
        p = strtokm(NULL, "$");
        cs.iterations = atoi(p);
        p = strtokm(NULL, "$");
        cs.tag_len = atoi(p) / 8;       p = strtokm(NULL, "$");
        cs.saltlen = atoi(p);
        p = strtokm(NULL, "$");
        for (i = 0; i < cs.saltlen; i++)
                cs.salt[i] = (atoi16[ARCH_INDEX(p[2*i])] << 4) | atoi16[ARCH_INDEX(p[2*i+1])];
        p = strtokm(NULL, "$");
        for (i = 0; i < 16; i++)
                cs.iv[i] = (atoi16[ARCH_INDEX(p[2*i])] << 4) | atoi16[ARCH_INDEX(p[2*i+1])];
        p = strtokm(NULL, "$");
        cs.addlen = atoi(p);
        p = strtokm(NULL, "$");
        for (i = 0; i < cs.addlen; i++)
                cs.add[i] = (atoi16[ARCH_INDEX(p[2*i])] << 4) | atoi16[ARCH_INDEX(p[2*i+1])];
        p = strtokm(NULL, "$");
        cs.ctlen = atoi(p);
        p = strtokm(NULL, "$");
        for (i = 0; i < cs.ctlen; i++)
                cs.ct[i] = (atoi16[ARCH_INDEX(p[2*i])] << 4) | atoi16[ARCH_INDEX(p[2*i+1])];
  
        MEM_FREE(keeptr);
  
        return (void *)&cs;
  }
  
  static void set_salt(void *salt)
  {
        cur_salt = (struct custom_salt *)salt;
  }
  
  static int crypt_all(int *pcount, struct db_salt *salt)
  {
        const int count = *pcount;
        int index = 0;
  
        memset(cracked, 0, sizeof(cracked[0])*cracked_count);
  
  
  #pragma omp parallel for
  
        for (index = 0; index < count; index += MIN_KEYS_PER_CRYPT) {
                unsigned char master[MIN_KEYS_PER_CRYPT][32];
                unsigned char output[4096] = {0};
                int i;
                unsigned char *tag = cur_salt->ct + cur_salt->ctlen - cur_salt->tag_len; 
                int lens[MIN_KEYS_PER_CRYPT];
                unsigned char *pin[MIN_KEYS_PER_CRYPT], *pout[MIN_KEYS_PER_CRYPT];
                for (i = 0; i < MIN_KEYS_PER_CRYPT; ++i) {
                        lens[i] = strlen(saved_key[index+i]);
                        pin[i] = (unsigned char*)saved_key[index+i];
                        pout[i] = master[i];
                }
                pbkdf2_sha256_sse((const unsigned char**)pin, lens, cur_salt->salt, cur_salt->saltlen, cur_salt->iterations, pout, 32, 0);
                for (i = 0; i < MIN_KEYS_PER_CRYPT; ++i)
                        pbkdf2_sha256((unsigned char *)saved_key[index+i], strlen(saved_key[index+i]), cur_salt->salt, cur_salt->saltlen, cur_salt->iterations, master[i], 32, 0);
  
  
                for (i = 0; i < MIN_KEYS_PER_CRYPT; ++i) {
                        memset(output, 0, 4096);                        aes_ccm_auth_decrypt(master[i], 256,
                                        cur_salt->ctlen - cur_salt->tag_len,
                                        cur_salt->iv, 13, cur_salt->add,                                        cur_salt->addlen, cur_salt->ct, output,
                                        tag, cur_salt->tag_len);
                                                                                                                        if (cur_salt->ctlen - cur_salt->tag_len == 2) {                              if (strncmp((const char*)output, "[]", 2) == 0)
                                        cracked[index+i] = 1;
                        } else {                                if (output[0] != '[')
                                        cracked[index+i] = 0;
                                else if (strstr((const char*)output, "\"updated\""))
                                        cracked[index+i] = 1;
                        }
                }
        }
  
        return count;
  }
  
  static int cmp_all(void *binary, int count)
  {
        int index;
  
        for (index = 0; index < count; index++)
                if (cracked[index])
                        return 1;
        return 0;
  }
  
  static int cmp_one(void *binary, int index)
  {
        return cracked[index];
  }
  
  static int cmp_exact(char *source, int index)
  {
        return 1;
  }
  
  static void set_key(char *key, int index)
  {
        saved_len[index] =
                strnzcpyn(saved_key[index], key, sizeof(saved_key[index]));
  }
  
  static char *get_key(int index)
  {
        return saved_key[index];
  }
  
  static unsigned int padlock_iteration_count(void *salt)
  {
        struct custom_salt *cs = salt;
  
        return (unsigned int) cs->iterations;
  }
  
  struct fmt_main fmt_padlock = {
        {
                FORMAT_LABEL,
                FORMAT_NAME,
                ALGORITHM_NAME,
                BENCHMARK_COMMENT,
                BENCHMARK_LENGTH,
                0,
                PLAINTEXT_LENGTH,
                BINARY_SIZE,
                BINARY_ALIGN,
                SALT_SIZE,
                SALT_ALIGN,
                MIN_KEYS_PER_CRYPT,
                MAX_KEYS_PER_CRYPT,
                FMT_CASE | FMT_8_BIT | FMT_OMP | FMT_HUGE_INPUT,
                {
                        "iteration count",
                },
                { FORMAT_TAG },
                tests
        }, {
                init,
                done,
                fmt_default_reset,
                fmt_default_prepare,
                valid,
                fmt_default_split,
                fmt_default_binary,
                get_salt,
                {
                        padlock_iteration_count,
                },
                fmt_default_source,
                {
                        fmt_default_binary_hash
                },
                fmt_default_salt_hash,
                NULL,
                set_salt,
                set_key,
                get_key,
                fmt_default_clear_keys,
                crypt_all,
                {
                        fmt_default_get_hash
                },
                cmp_all,
                cmp_one,
                cmp_exact
        }
  };