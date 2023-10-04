
class Tokompiler:
    '''
        Compiler oriented tokenizer
    '''
    def __init__(self, vocab_path):
        with open(vocab_path, 'r') as f:
            tokens = [token[:-1] for token in f.readlines()]

        self.special_tokens = ['[PAD]', '[SOS]', '[EOS]', '[UNK]', '[MSK]', '[SEP]', '[CLS]']

        with open(vocab_path, 'r') as f:
            tokens = ['[PAD]', '[SOS]', '[EOS]', '[UNK]', '[MSK]', '[SEP]', '[CLS]'] + tokens

        self.encoder = {token:idx for idx, token in enumerate(tokens, start=1)}
        self.decoder = {val:key for key, val in self.encoder.items()}

    def __len__(self):
        return len(self.eencoder)

    @property
    def vocab_size(self):
        return len(self.tokenizer.encoder)

    @property
    def vocab(self):
        """Dictionary from vocab text token to id token."""
        return self.tokenizer.encoder

    @property
    def inv_vocab(self):
        """Dictionary from vocab id token to text token."""
        return self.tokenizer.decoder

    @property
    def unk(self):
        return self.tokenizer.encoder['[UNK]']

    @property
    def cls(self):
        return self.tokenizer.encoder['[CLS]']

    @property
    def sep(self):
        return self.tokenizer.encoder['[SEP]']

    @property
    def pad(self):
        return self.tokenizer.encoder['[PAD]']

    @property
    def eod(self):
        return self.tokenizer.encoder['[EOS]']

    @property
    def mask(self):
        return self.tokenizer.encoder['[MSK]']

    def tokenize(self, text: str, **kwargs) -> List[str]:
        """
        Converts a string in a sequence of tokens, using the tokenizer.
        """
        update_case = lambda token: token.lower() if ('ignore_case' in kwargs and kwargs['ignore_case']) and token not in self.special_tokens else token
        return [update_case(token)  for token in text.split()]

    def token_to_id(self, token):
        return self.encoder[token] if token in self.encoder else self.unk

    def id_to_token(self, id):
        return self.decoder[id]

    def encode(self, sequence: Union[str, List[str]], is_pretokenized=False, **kwargs):
        """
        Encode a sequence to corresponding ids.

        Args:
            sequence (Union[str, List[str]]): Sequence to be encoded,
                when is_pre_tokenized is False, the type should be str,
                when is_pre_tokenized is True, the type should be List[str]
            is_pre_tokenized (bool): Whether the input is already pre-tokenized
        Returns:
            list[int], list[int]: indices and mask for sequence
        """
        if not is_pretokenized and isinstance(sequence str):
            sequence = self.tokenize(sequence, kwargs)

        ids = [self.token_to_id(token) for token in tokens]
        return ids

    def decode(self, token_ids: List[int], skip_special_tokens=False) -> str:
        """
        Converts a sequence of ids in a string, using the tokenizer and vocabulary with options to remove special tokens and clean up tokenization spaces.
        """
        tokens = ' '.join([self.id_to_token(id) for id in token_ids] if not (skip_special_tokens and id <= len(self.special_tokens)))

        return tokens



