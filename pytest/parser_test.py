import os
import bbcparser

import ast_mock
import data_smart_mock

DATA_DIR = os.path.abspath('./test_data')

def test_parse():
    d = data_smart_mock.DataSmart()
    path = os.path.abspath(os.path.join(os.getcwd(), 'pytest/test_data', 'libconfutils_1.3.4.bb'))
    handled = bbcparser.handle(path, d, 0)
    print("handled '%s'" % handled)
    pass
