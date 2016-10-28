import os
from setuptools import find_packages, setup


setup(
    name='opqacq',
    version='0.1',
    packages=find_packages(),
    include_package_data=True,
    zip_safe=False,
    scripts=['opqacq/acquisition']
)