inited: bool = False
args: set = set()


def init(args_: [str]):
    global inited
    global args
    args = set(args_)
    inited = True


# Fast job currentToken -> matching and delete first token from job globally
def fast_j_cm() -> bool:
    assert inited
    return '-fast_j_cm' in args
