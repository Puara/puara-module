# Git History Guide for Puara Module

This guide explains how to view the full history of merges and commits in the puara-module repository, including how to search for specific changes.

## Viewing Full Commit History

### Basic commit history
To view all commits in chronological order:
```bash
git log --all --oneline
```

### Detailed commit history with graph
To see a visual representation of branches and merges:
```bash
git log --all --graph --decorate --oneline
```

For more detailed information:
```bash
git log --all --graph --decorate --pretty=format:'%C(yellow)%h%Creset %C(cyan)%an%Creset %C(green)(%ar)%Creset %s'
```

## Viewing Merge History

### List all merge commits
```bash
git log --all --merges --oneline
```

### Detailed merge commits with parent information
```bash
git log --all --merges --pretty=format:'%h - %s (%an, %ar)'
```

### View a specific merge commit details
```bash
git show <commit-hash>
```

## Searching for Specific Changes

### Search for commits mentioning "OSC" in commit messages
```bash
git log --all --grep="OSC" --oneline
```

### Search for commits that modified OSC-related files
```bash
git log --all -S"osc" -- "*.json"
git log --all -- data/settings.json
```

### Search for actual code changes containing "OSC"
To find commits where the actual diff contains "OSC":
```bash
git log --all -S"OSC" --source
```

Or with more context:
```bash
git log --all -S"OSC" -p
```

### View changes to a specific file
```bash
git log --all -p -- data/settings.json
```

## Filtering by Author

### Find all commits by Edu (or any author)
```bash
git log --all --author="Edu" --oneline
```

Variations to try:
```bash
git log --all --author="Meneses" --oneline
git log --all --author="Eduardo" --oneline
git log --all --author="edumeneses" --oneline
```

### Find commits by Edu that mention OSC
```bash
git log --all --author="Edu" --grep="OSC" --oneline
```

### Find commits by Edu that changed settings files
```bash
git log --all --author="Edu" -- data/settings.json
```

## Working with Shallow Clones

If you're working with a shallow clone (limited history), you may need to fetch the full history:

### Check if repository is shallow
```bash
git rev-parse --is-shallow-repository
```

### Convert shallow clone to full clone
```bash
git fetch --unshallow
```

Or specify a depth:
```bash
git fetch --depth=1000
```

## Advanced History Analysis

### View file history with diffs
```bash
git log -p -- data/settings.json
```

### Compare two branches
```bash
git log main..feature-branch --oneline
```

### Find when a specific line was changed
```bash
git log -S"OSC_IP" -p -- data/settings.json
```

### View commits between two dates
```bash
git log --all --since="2023-01-01" --until="2024-12-31" --oneline
```

### See who changed what in a file
```bash
git blame data/settings.json
```

## Finding Removed Content

### Search for deleted content
To find when "OSC_IP" or other OSC settings were removed:
```bash
git log --all -S"OSC_setting_name" --diff-filter=D
```

### View all deletions in a file
```bash
git log --all --diff-filter=D -- data/settings.json
```

### Show what was deleted in a specific commit
```bash
git show <commit-hash>
```

## Using GitHub Web Interface

If git commands are not sufficient, you can also use GitHub's web interface:

1. **View commits**: `https://github.com/Puara/puara-module/commits/main`
2. **View pull requests**: `https://github.com/Puara/puara-module/pulls?q=is%3Apr`
3. **Search commits**: Use GitHub's search with `author:edumeneses` or `OSC`
4. **View file history**: Navigate to a file and click "History" button
5. **Network graph**: `https://github.com/Puara/puara-module/network`

## Specific Use Case: Finding Edu's OSC Settings Removal

To find where Edu removed OSC settings, try these commands in sequence:

```bash
# 1. Ensure you have full history
git fetch --unshallow

# 2. Search for OSC-related commits by Edu
git log --all --author="Edu" -S"OSC" --oneline

# 3. View detailed changes in settings file
git log --all -p -- data/settings.json

# 4. Search for deleted OSC settings
git log --all -S"OSC" --diff-filter=D -p

# 5. Check pull requests that were merged
git log --all --merges --grep="OSC" --oneline
```

### Example: OSC Settings Removal History

In the puara-module repository, OSC settings were removed in the following commits:

1. **Commit 837664b** (June 11, 2025) by Sara Alamoun:
   - Removed OSC trails from `data/config.json`
   - Removed: `oscIP1`, `oscPORT1`, `oscIP2`, `oscPORT2`, `localPORT`
   ```bash
   git show 837664b
   ```

2. **Pull Request #23** (November 21, 2025) merged by Edu Meneses:
   - Title: "Remove OSC from config"
   - Merged the `feature/move-osc` branch
   ```bash
   git show 9662c32
   ```

3. **Commit 751b4dd** (November 25, 2025) by Charles B:
   - Updated with 'OSC-move' branch which removes all OSC from configs
   ```bash
   git show 751b4dd
   ```

4. **Pull Request #29** merged by Edu:
   - Later added back OSC settings to `data/settings.json`
   - Added: `OSC_IP`, `OSC_port`, `OSC_local_port`
   ```bash
   git show f4f08e0
   git show db27515
   ```

To see the full history of OSC-related changes:
```bash
git log --all -S"OSC" --oneline --name-status
```

## Tips

- Use `--oneline` for compact output
- Use `-p` to see actual changes (patches)
- Use `--stat` to see file change statistics
- Add `| less` to page through long output: `git log --all | less`
- Use `q` to quit from less/pager view
- Combine multiple filters for precise searches

## Need More Help?

- Git documentation: https://git-scm.com/docs
- GitHub documentation: https://docs.github.com/
- Contact repository maintainers for specific historical questions
