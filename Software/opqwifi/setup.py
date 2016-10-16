from setuptools import setup, find_packages


setup(name='opqwifi',
    version='0.1',
    description='Network manager web frontend.',
    author='Sergey Negrashov',
    author_email='sin8 at hawaii dot edu',
    url='https://github.com/openpowerquality/opqbox2',
    packages=find_packages(),
    include_package_data=True,
    scripts=['opqwifi/opqwifi'],
    install_requires=[
        "bottle",
    ],
)
