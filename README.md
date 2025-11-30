 shared ⚙️
> This repository is the central organization for every Monitio project.
> It defines the universal standards, policies, configuration files and
> guidelines that apply to every single repository within the Monitio
> organization.
>
> If you decide to transfer an existing project into Monitio, the
> information here is ***mandatory***.

---

> [!WARNING]
> Not all information here is fully finished yet. The [`CONTRIBUTING.md`](CONTRIBUTING.md) file is only half done and the [`CODE_OF_CONDUCT.md`](CODE_OF_CONDUCT.md) has not even been made yet. Contact info will also definitely be wrong until some severe maintenance changes are made.

---

## Standards
All Monitio projects must adhere to these policies and guidelines, which are managed directly here in this repository.
  - **Contribution Guidelines** ([`CONTRIBUTING.md`](CONTRIBUTING.md))
    - Defines the process for submitting Pull Requests and code quality.
  - **Community Standards** ([`CODE_OF_CONDUCT.md`](CODE_OF_CONDUCT.md))
    - Outlines the behavioral guidelines expected of all contributors and non-contributors.
  - **Licensing Policy** ([`LICENSE.md`](LICENSE.md))
    - All Monitio code is licensed under the [**GNU General Public License v3.0 (GPL-3.0)**](https://www.gnu.org/licenses/gpl-3.0.en.html#license-text).

## Mandatory Technical Configuration
These files should be used to stay up to standards on the **Contribution Guidelines** ([`CONTRIBUTING.md`](CONTRIBUTING.md)). Before release of a project, specific formatting tools listed here should be used to format code, then you can commit the clean code and compile ready for release.
  - [`templates/.prettierrc`](templates/.prettierrc) *(may get replaced)*
    - Enforces consistent style across all web-based languages.
  - [`scripts/todo.py`](scripts/todo.py) *(may get replaced)*
    - An automated script for tracking multiple different types of `TODO` comments in every language using python.
  - [`VERSIONING.md`](VERSIONING.md) *(permanent)*
    - This is used as a way to show what versioning scheme we are using. In Monitio projects, it is **mandatory** to use the [*SemVer Typing*](https://github.com/monitio/semver-typing) versioning scheme.

## How to use this repository properly 🧭
All projects should use the most updated files in the `templates/` directory to improve organization. Projects should regularly check this repository for updates to standards.
1. **Documentation**: Copy the [`LICENSE.md`](LICENSE.md), [`CODE_OF_CONDUCT.md`](CODE_OF_CONDUCT.md) and [`CONTRIBUTING.md`](CONTRIBUTING.md) files into your project root.
2. **Configuration**: Copy the necessary configuration files (e.g., [`templates/.prettierrc`](templates/.prettierrc)) into your project root.
3. **Scripts**: Integrate other useful Monitio projects or the [scripts found in this repo](scripts/) into your project's pre-commit hooks or CI workflow.

---

### Contacts
[moniti.org](https://moniti.org/)
[hello@moniti.org](mailto:hello@moniti.org)

