import os
from urllib.parse import urlparse
from git import Repo, Remote

if __name__ == "__main__":
    repo_path = os.getenv("GIT_REPO_PATH")
    repo = Repo(repo_path)

    assert not repo.bare

    while True:
        remote_url_query = "What is your remote repository?: "
        remote_url = input(remote_url_query)
        while not remote_url:
            remote_url = input(remote_url_query)

        name_query = "What is the name of your project?: "
        name = input(name_query)
        while not name:
            name = input(name_query)

        confirm_query = "Is this information correct? \n" + "Remote url: " + remote_url + "\n" + "Project name: " + name + "\n" + "(Y/N): "
        confirm_response = input(confirm_query).lower()
        while confirm_response != "y" and confirm_response != "n":
            confirm_response = input(confirm_query).lower()
        
        if confirm_response == "n":
            continue
        else:
            break
    
    # Set remote url
    remote = Remote(repo, "origin")
    if remote:
        remote.set_url(remote_url)
    else:
        remote = repo.create_remote("origin", url=remote_url)

    # Pull submodules
    for submodule in repo.submodules:
        submodule.update(init=True)

    # Set project name:
    name = "_".join(name.split())
    cmake_lists = "CMakeLists.txt"
    with open(cmake_lists) as f:
        s = f.read()

    with open(cmake_lists, "w") as f:
        replaced = s.replace("vulkan_project", name)
        f.write(replaced)