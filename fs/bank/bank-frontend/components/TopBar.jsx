import logo from '../assets/icons/bank-one-one-logo.png';

export default function TopBar() {
  return (
    <header className="app-topbar">
      <div className="app-topbar__inner">
        <div className="app-brand" aria-label="Bank One One">
          <img src={logo} alt="Bank One One logo" />
          <span>Bank One One</span>
        </div>
      </div>
    </header>
  );
}
