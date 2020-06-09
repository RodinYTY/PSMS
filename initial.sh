#!/usr/bin/env bash 
git init
git config --global user.name "RodinYTY"
git config --global user.email "1418583869@qq.com"
ssh-keygen -t rsa -C "1418583869@qq.com"
echo "Paste the id_rsa key on github SSH"
cat ~/.ssh/id_rsa.pub

read -r -p "Repository name? " string
git remote add origin git@github.com:RodinYTY/$string.git
echo "$string.git added"

read -r -p "Continue to initial your repository? [Y/n] " input
case $input in
    [yY][eE][sS]|[yY])
		;;

    *)
		exit 1
		;;
esac

echo "pulling origin master..."
git pull origin master --allow-unrelated-histories
git add .

read -r -p "Input comment for committing " comment             
git commit -m "$comment"

git push origin master
