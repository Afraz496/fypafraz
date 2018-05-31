ifeq($(OS),MachOS)
brew install gmp
brew install mpfr
else
sudo apt-get install gmp
sudo apt-get install mpfr
