from git import Repo

repo = Repo(self.rorepo.working_tree_dir)
assert not repo.bare

