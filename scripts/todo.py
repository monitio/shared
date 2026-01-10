#!/usr/bin/env python3
import sys
import re
import os

# -- ANSI Terminal Colors and Formatting
class Colors:
  HEADER = '\033[95m'
  OKBLUE = '\033[94m'
  OKGREEN = '\033[92m'
  WARNING = '\033[93m'
  FAIL = '\033[91m'
  ENDC = '\033[0m'
  BOLD = '\033[1m'
  UNDERLINE = '\033[4m'

# -- Config
TYPES = ("TODO", "FIXME", "BUG", "DANGER", "TODOCS")

# -- Files
OUTPUT_FILE = "TODO.md"
README_FILE = "README.md"

# -- Exclusion List
EXCLUDE_DIRS = {".venv", "node_modules", "dist", ".git", "__pycache__"}
EXCLUDE_FILES = {OUTPUT_FILE, README_FILE, os.path.basename(sys.argv[0])}

# -- Markers
START_MARKER = "<!-- TODOS_START -->"
END_MARKER = "<!-- TODOS_END -->"

# -- Regex Pattern
TYPE_PATTERN = '|'.join(re.escape(t) for t in TYPES)
TODO_REGEX = re.compile(
  # Match type, label in parentheses, optional colon, and message up to the newline.
  fr'({TYPE_PATTERN})\s*\(([^)]*)\)(?:\s*:\s*([^\n]*))?',
  re.IGNORECASE
)

def get_header_text(todo_type: str) -> str:
  """Determine the header text based on the todo type."""
  if todo_type == "FIXME":
    return "'FIXME's"
  elif todo_type == "TODOCS":
    return "TODOCSes"
  return f"{todo_type}s"

def print_styled(message: str, style: str = Colors.ENDC, indent: int = 0):
  """Prints a message with optional styling and indentation to stderr."""
  indent_str = ' ' * indent
  # Use sys.stderr for all console output
  print(f"{style}{indent_str}{message}{Colors.ENDC}", file=sys.stderr)

def extract_todos_from_project(root_dir=".") -> list:
  """
  Walks the project directory, extracts TODOs using regex, and returns
  a list of dictionaries, one for each found todo.
  """
  print_styled(f"\n{Colors.BOLD}Starting Project Scan{Colors.ENDC}", style=Colors.HEADER)

  styled_types = ", ".join(f"{Colors.BOLD}{t}{Colors.ENDC}" for t in TYPES)
  print_styled(f"> Types targeted: {styled_types}", indent=2)

  todos_list = []

  for dirpath, dirnames, filenames in os.walk(root_dir):
    # Skip excluded directories
    dirnames[:] = [d for d in dirnames if d not in EXCLUDE_DIRS and not d.startswith('.')]

    for filename in filenames:
      if filename in EXCLUDE_FILES:
        continue
        
      file_path = os.path.join(dirpath, filename)

      try:
        # Read file content line-by-line for robust regex matching
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
          for line in f:
            for match in TODO_REGEX.finditer(line):
              todo_type = match.group(1).upper()
              label = match.group(2).strip()
              message = match.group(3).strip() if match.group(3) else ""
              
              todos_list.append({
                "path": file_path,
                "type": todo_type,
                "label": label,
                "message": message,
              })
        
      except IOError:
        print_styled(f"Warning: Could not read file: {file_path}", style=Colors.WARNING, indent=4)
        continue
        
  # Use os.path.relpath to make paths relative to the current directory
  for item in todos_list:
    item['path'] = os.path.relpath(item['path'])

  count = len(todos_list)
  print_styled(f"> Found {Colors.BOLD}{count}{Colors.ENDC} total comments.", indent=2)
  return todos_list

def generate_report() -> bool:
  """
  Extracts all todos and generates the TODO.md report file.
  """
  todos_data = extract_todos_from_project()
  
  if not todos_data:
    print_styled("\nNo outstanding comments found. Creating clean report.", style=Colors.OKBLUE)
    try:
       with open(OUTPUT_FILE, 'w', encoding='utf-8') as f:
        f.write("# No outstanding todos!\n")
       return True
    except IOError as e:
      print_styled(f"Error writing to output file {OUTPUT_FILE}: {e}", style=Colors.FAIL, indent=2)
      return False

  # 4. Generate the report (TODO.md)
  try:
    processed_lines = set()
    
    with open(OUTPUT_FILE, 'w', encoding='utf-8') as f:
      first_block = True

      for todo_type in TYPES:
        filtered_todos = [item for item in todos_data if item.get('type') == todo_type]
        
        # Always print the section header even if there is no comments from that type
        if not first_block:
          f.write("\n---\n\n")

        header_text = get_header_text(todo_type)
        f.write(f"# {header_text}\n")
        
        if not filtered_todos:
          f.write("\n") 

        first_block = False

        for item in filtered_todos:
          path_part = ""
          if item.get('path'):
            path_part = f"({item['path']}) - "
          
          label = item.get('label', '')
          message = item.get('message', '')
          
          separator = ": " if message else ""
          
          line = f"- [ ] {path_part}{label}{separator}{message}"
          
          if line not in processed_lines:
            f.write(f"{line}\n")
            processed_lines.add(line)

  except IOError as e:
    print_styled(f"Error writing to output file {OUTPUT_FILE}: {e}", style=Colors.FAIL, indent=2)
    return False

  print_styled(f"\n{Colors.BOLD}Report Generation Success{Colors.ENDC}", style=Colors.OKGREEN)
  print_styled(f"> Full report successfully generated in {Colors.BOLD}{OUTPUT_FILE}{Colors.ENDC}", indent=2)
  return True

def update_readme_link() -> bool:
  """
  Reads README.md, replaces the content between the markers with the 
  TODO.md link on a new line, and writes the changes back.
  """
  LINK_CONTENT = f'This project also has a [\"todo list \"]({OUTPUT_FILE}).'

  if not os.path.exists(README_FILE):
    print_styled(f"Error: the '{README_FILE}' file doesn't exist", style=Colors.FAIL)
    print_styled(f"Please make a '{README_FILE}' file and add the markers.", style=Colors.FAIL, indent=2)
    print_styled(f"The markers are: '{START_MARKER}' and '{END_MARKER}'", style=Colors.FAIL, indent=2)
    return False

  try:
    with open(README_FILE, 'r', encoding='utf-8') as f:
      content = f.readlines()
  except IOError as e:
    print_styled(f"Error reading {README_FILE}: {e}", style=Colors.FAIL, indent=2)
    return False

  has_start = any(line.strip() == START_MARKER for line in content)
  has_end = any(line.strip() == END_MARKER for line in content)

  if not has_start or not has_end:
    print_styled(f"Error: the '{README_FILE}' file doesn't have the markers in it yet.", style=Colors.FAIL)
    print_styled(f"Please add the markers to your '{README_FILE}'.", style=Colors.FAIL, indent=2)
    print_styled(f"The markers are: '{START_MARKER}' and '{END_MARKER}'", style=Colors.FAIL, indent=2)
    return False

  new_content = []
  in_todos_block = False
  
  for line in content:
    line_stripped = line.strip()

    if line_stripped == START_MARKER:
      new_content.append(START_MARKER + '\n')
      # Insert link content here, on its own line.
      new_content.append(LINK_CONTENT + '\n') 
      in_todos_block = True
      continue 
    
    if line_stripped == END_MARKER:
      new_content.append(END_MARKER + '\n')
      in_todos_block = False
      continue

    if in_todos_block: 
      continue

    new_content.append(line)
  
  try:
    with open(README_FILE, 'w', encoding='utf-8') as f:
      f.writelines(new_content)
  except IOError as e:
    print_styled(f"Error writing to {README_FILE}: {e}", style=Colors.FAIL, indent=2)
    return False

  print_styled(f"\n{Colors.BOLD}README Update Success{Colors.ENDC}", style=Colors.OKBLUE)
  print_styled(f"> Successfully updated link within {Colors.BOLD}{README_FILE}{Colors.ENDC} markers.", indent=2)
  return True

def main():
  """Main execution function."""
  if not generate_report():
    sys.exit(1)

  if not update_readme_link():
    sys.exit(1)

if __name__ == "__main__":
  main()
