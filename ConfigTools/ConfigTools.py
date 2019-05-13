'''
2019.05    @Aeonni
ConfigTools.py
https://aeonni.com
'''

import os

__all__ = ["ReadConfig", "SaveConfig"]


class param_dict:
    def __repr__(self):
        return self.__dict__.__repr__()

    def __get_dict__(self):
        d = self.__dict__
        for k, v in d.items():
            if type(v) == param_dict:
                d[k] = v.__get_dict__()
        return d


def _iter_obj(obj):
    if type(obj) == dict:
        new_param_dict = param_dict()
        for k, v in obj.items():
            obj[k] = _iter_obj(v)
        new_param_dict.__dict__.update(obj)
        return new_param_dict
    return obj


def ReadConfig(filename, base=os.getcwd(), raw=False, verbose=True):
    file_path = os.path.join(os.path.expandvars(base), filename)
    if verbose:
        print('Reading configuration in : ', file_path)
    with open(file_path, 'r') as f:
        text = f.read()
    if filename.endswith(('.yaml', '.yml')):
        import yaml
        cfg = yaml.load(text)
    elif filename.endswith(('.json')):
        import json
        cfg = json.loads(text)
    else:
        raise("File type not supported!")
    if raw:
        return cfg
    return _iter_obj(cfg)


def SaveConfig(obj, filename=None, base=os.getcwd(), type_=None, verbose=True):
    '''
    function SaveConfig:
        Save your configurations to file.
        If filename is set to be None, you should set type_ as 'json' or 'yaml'.
    '''
    if (filename.endswith(('.yaml', '.yml')) if filename is not None else type_ == 'yaml'):
        import yaml
        text = yaml.dump(obj.__get_dict__())
    elif (filename.endswith(('.json')) if filename is not None else type_ == 'json'):
        import json
        text = json.dumps(obj.__get_dict__(), sort_keys=True)
    else:
        raise TypeError("File type not supported!")

    if filename is not None:
        file_path = os.path.join(os.path.expandvars(base), filename)
        if verbose:
            print('Saving configuration to path : ', file_path)
        with open(file_path, 'w') as f:
            f.write(text)
    return text


def GetPraser(obj, description, prased=True):
    assert type(obj) == param_dict
    args = obj.__PRASER_ARGS__.__get_dict__()
    import argparse
    parser = argparse.ArgumentParser(description=description)
    for k, v in args.items():
        l = ['--'+k if len(k) > 1 else '-'+k]
        try:
            if v.get('short'):
                l.append('-'+k[0])
        except:
            pass

        if v is None or type(v) == dict:
            parser.add_argument(*l, default=obj.__getattribute__(k), type=type(
                obj.__getattribute__(k)), help=v.get('help') if v is not None and v.get('help') is not None else '')
        else:
            parser.add_argument(
                *l, dest=k, action='store_false' if v else 'store_true')
    if prased:
        return parser.parse_args()
    return parser


def MergeOptions(obj, arg):
    obj.__dict__.update(arg.__dict__)
    obj.__delattr__('__PRASER_ARGS__')
    return obj


def ReadAndPrase(filename, description, base=os.getcwd()):
    '''
    function ReadAndPrase:
        It will do all the work you need to be done to prepare your configurations.
    '''
    opt = ReadConfig(filename, base, verbose=False)
    arg = GetPraser(opt, description)
    return MergeOptions(opt, arg)


if __name__ == '__main__':
    # print('Config Tools ver.0.0.3')
    if os.path.exists('cfg.yaml'):
        # print('')
        opt = ReadAndPrase('cfg.yaml', 'TEST')
        print(opt)
